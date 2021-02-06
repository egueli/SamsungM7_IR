#include "config.h"

#include "mdns.h"
#include "wifi.h"
#include "discovery.h"

#include <Arduino.h>
#include <mdns.h>
#include <map>
#include <set>

/**
 * The MDNS query to send the network to find the speaker.
 */
const char *kServiceQuestion = kSpeakerMdnsServiceQuestion;

const char *kServiceName = kSpeakerServiceName;
const unsigned int kMaxMDNSPacketSize = 512;
byte buffer[kMaxMDNSPacketSize];

bool querySent;

const size_t kMaxEntries = 16;
std::set<String> services;
std::map<String, String> serviceToHost;
std::map<String, String> hostToAddress;

String ipAddress;

void dumpServices();

// When an mDNS packet gets parsed this callback gets called once per Query.
// See mdns.h for definition of mdns::Query.
void answerCallback(const mdns::Answer *answer)
{
    // A typical PTR record matches service to a human readable name.
    // eg:
    //  service: _mqtt._tcp.local
    //  name:    Mosquitto MQTT server on twinkle.local
    if (answer->rrtype == MDNS_TYPE_PTR and strstr(answer->name_buffer, kServiceQuestion) != 0)
    {
        if (services.size() >= kMaxEntries)
        {
            Serial.println("got too many services, ignoring");
        }
        else
        {
            services.insert(String(answer->rdata_buffer));
        }
    }

    // A typical SRV record matches a human readable name to port and FQDN info.
    // eg:
    //  name:    Mosquitto MQTT server on twinkle.local
    //  data:    p=0;w=0;port=1883;host=twinkle.local
    if (answer->rrtype == MDNS_TYPE_SRV)
    {
        if (serviceToHost.size() >= kMaxEntries)
        {
            Serial.println("got too many interesting SRV responses, ignoring");
        }
        else
        {
            char *port_start = strstr(answer->rdata_buffer, "port=");
            if (port_start)
            {
                port_start += 5;
                char *port_end = strchr(port_start, ';');
                char port[1 + port_end - port_start];
                strncpy(port, port_start, port_end - port_start);
                port[port_end - port_start] = '\0';

                if (port_end)
                {
                    char *host_start = strstr(port_end, "host=");
                    if (host_start)
                    {
                        host_start += 5;
                        serviceToHost.insert({String(answer->name_buffer), String(host_start)});
                    }
                    else
                    {
                        Serial.println("malformed answer (no 'host=' found)");
                    }
                }
                else
                {
                    Serial.println("malformed answer (no port delimiter found)");
                }
            }
            else
            {
                Serial.println("malformed answer (no 'port=' found)");
            }
        }
    }

    if (answer->rrtype == MDNS_TYPE_A)
    {
        if (hostToAddress.size() >= kMaxEntries)
        {
            Serial.println("got too many interesting A responses, ignoring");
        }
        hostToAddress.insert({String(answer->name_buffer), String(answer->rdata_buffer)});
    }
}

mdns::MDns my_mdns(NULL, NULL, answerCallback, buffer, kMaxMDNSPacketSize);

void setupDiscovery()
{
}

void sendQuery(unsigned int type, const char *queryName)
{
    my_mdns.Clear();
    struct mdns::Query query;
    strncpy(query.qname_buffer, queryName, MAX_MDNS_NAME_LEN);
    query.qtype = type;
    query.qclass = 1; // "INternet"
    query.unicast_response = 0;
    my_mdns.AddQuery(query);
    my_mdns.Send();
}

bool serviceNameMatches(String &service)
{
    if (!kServiceName)
    {
        return true;
    }

    const int indexOfFirstDot = service.indexOf(".");
    if (indexOfFirstDot == -1)
    {
        // dot not found, so actually the passed service may be malformed
        return false;
    }

    const String serviceName = service.substring(0, indexOfFirstDot);
    return serviceName.equalsIgnoreCase(kServiceName);
}

void loopDiscovery()
{
    // Did we do our job already?
    if (!ipAddress.isEmpty())
    {
        return;
    }
    // Can we actually search?
    if (!isWifiConnected())
    {
        return;
    }
    if (!querySent)
    {
        sendQuery(MDNS_TYPE_PTR, kServiceQuestion);
        querySent = true;
    }
    my_mdns.loop();

    dumpServices();

    for (auto service : services)
    {
        auto hostIt = serviceToHost.find(service);

        if (hostIt != serviceToHost.end())
        {
            String host = hostIt->second;
            auto addrIt = hostToAddress.find(host);
            if (addrIt != hostToAddress.end())
            {
                if (serviceNameMatches(service))
                {
                    ipAddress = addrIt->second;
                    Serial.printf("FOUND service '%s' at host '%s' with address '%s'\n", service.c_str(), host.c_str(), addrIt->second.c_str());
                    onDiscoveryFinished(ipAddress);
                }
            }
        }
    }
}

void dumpServices()
{
    static unsigned long lastDumpAt;
    unsigned long now = millis();
    if (now < lastDumpAt + 1000L)
    {
        return;
    }
    lastDumpAt = now;

    Serial.printf("services map has now %d entries\n", services.size());
    for (auto service : services)
    {
        Serial.printf("  service '%s'\n", service.c_str());
    }
    Serial.printf("serviceToHost map has now %d entries\n", serviceToHost.size());
    for (auto serviceToHostEntry : serviceToHost)
    {
        Serial.printf("  service '%s' to host '%s'\n", serviceToHostEntry.first.c_str(), serviceToHostEntry.second.c_str());
    }
    Serial.printf("hostToAddress map has now %d entries\n", hostToAddress.size());
    for (auto hostToAddressEntry : hostToAddress)
    {
        Serial.printf("  host '%s' to address '%s'\n", hostToAddressEntry.first.c_str(), hostToAddressEntry.second.c_str());
    }

    Serial.printf("---------\n");
}