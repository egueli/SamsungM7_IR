#include "mdns.h"
#include "wifi.h"

#include <Arduino.h>
#include <mdns.h>

// TODO convert to constants
#define QUESTION_SERVICE "_spotify-connect._tcp.local"
#define MAX_MDNS_PACKET_SIZE 512

#define MAX_HOSTS 4
#define HOSTS_SERVICE_NAME 0
#define HOSTS_PORT 1
#define HOSTS_HOST_NAME 2
#define HOSTS_ADDRESS 3
String hosts[MAX_HOSTS][4]; // Array containing information about hosts received over mDNS.

// When an mDNS packet gets parsed this callback gets called once per Query.
// See mdns.h for definition of mdns::Query.
void answerCallback(const mdns::Answer *answer)
{
    Serial.printf("Got MDNS answer 0x%02d\n", answer->rrtype);
    // A typical PTR record matches service to a human readable name.
    // eg:
    //  service: _mqtt._tcp.local
    //  name:    Mosquitto MQTT server on twinkle.local
    if (answer->rrtype == MDNS_TYPE_PTR and strstr(answer->name_buffer, QUESTION_SERVICE) != 0)
    {
        unsigned int i = 0;
        for (; i < MAX_HOSTS; ++i)
        {
            if (hosts[i][HOSTS_SERVICE_NAME] == answer->rdata_buffer)
            {
                // Already in hosts[][].
                break;
            }
            if (hosts[i][HOSTS_SERVICE_NAME] == "")
            {
                // This hosts[][] entry is still empty.
                hosts[i][HOSTS_SERVICE_NAME] = answer->rdata_buffer;
                break;
            }
        }
        if (i == MAX_HOSTS)
        {
            Serial.print(" ** ERROR ** No space in buffer for ");
            Serial.print('"');
            Serial.print(answer->name_buffer);
            Serial.print('"');
            Serial.print("  :  ");
            Serial.print('"');
            Serial.println(answer->rdata_buffer);
            Serial.print('"');
        }
    }

    // A typical SRV record matches a human readable name to port and FQDN info.
    // eg:
    //  name:    Mosquitto MQTT server on twinkle.local
    //  data:    p=0;w=0;port=1883;host=twinkle.local
    if (answer->rrtype == MDNS_TYPE_SRV)
    {
        unsigned int i = 0;
        for (; i < MAX_HOSTS; ++i)
        {
            if (hosts[i][HOSTS_SERVICE_NAME] == answer->name_buffer)
            {
                // This hosts entry matches the name of the host we are looking for
                // so parse data for port and hostname.
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
                            hosts[i][HOSTS_PORT] = port;
                            hosts[i][HOSTS_HOST_NAME] = host_start;
                        }
                    }
                }
                break;
            }
        }
        if (i == MAX_HOSTS)
        {
            Serial.print(" Did not find ");
            Serial.print('"');
            Serial.print(answer->name_buffer);
            Serial.print('"');
            Serial.println(" in hosts buffer.");
        }
    }

    // A typical A record matches an FQDN to network ipv4 address.
    // eg:
    //   name:    twinkle.local
    //   address: 192.168.192.9
    if (answer->rrtype == MDNS_TYPE_A)
    {
        int i = 0;
        for (; i < MAX_HOSTS; ++i)
        {
            if (hosts[i][HOSTS_HOST_NAME] == answer->name_buffer)
            {
                hosts[i][HOSTS_ADDRESS] = answer->rdata_buffer;
                break;
            }
        }
        if (i == MAX_HOSTS)
        {
            Serial.print(" Did not find ");
            Serial.print('"');
            Serial.print(answer->name_buffer);
            Serial.print('"');
            Serial.println(" in hosts buffer.");
        }
    }

    Serial.println();
    for (int i = 0; i < MAX_HOSTS; ++i)
    {
        if (hosts[i][HOSTS_SERVICE_NAME] != "")
        {
            Serial.print(">  ");
            Serial.print(hosts[i][HOSTS_SERVICE_NAME]);
            Serial.print("    ");
            Serial.print(hosts[i][HOSTS_PORT]);
            Serial.print("    ");
            Serial.print(hosts[i][HOSTS_HOST_NAME]);
            Serial.print("    ");
            Serial.println(hosts[i][HOSTS_ADDRESS]);
        }
    }
}

byte buffer[MAX_MDNS_PACKET_SIZE];
mdns::MDns my_mdns(NULL, NULL, answerCallback, buffer, MAX_MDNS_PACKET_SIZE);

void setupDiscovery()
{
}

bool querySent;

void sendQuery() {
    // Query for all host information for a paticular service. ("_mqtt" in this case.)
    my_mdns.Clear();
    struct mdns::Query query;
    strncpy(query.qname_buffer, QUESTION_SERVICE, MAX_MDNS_NAME_LEN);
    query.qtype = MDNS_TYPE_PTR;
    query.qclass = 1; // "INternet"
    query.unicast_response = 0;
    my_mdns.AddQuery(query);
    my_mdns.Send();
}

void loopDiscovery()
{
    if (!querySent && isWifiConnected()) {
        Serial.println("sending MDNS query");
        sendQuery();
        querySent = true;
    }
    my_mdns.loop();
}