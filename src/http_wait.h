#pragma once

/**
 * Called when an HTTP request has been sent and a response is being awaited.
 * The body of this function can call functions that have to be called very
 * frequently and cannot wait for a HTTP request to be fully completed.
 * The functions should not call the same function that originated the HTTP
 * request, unless it's reentrant.
 */
void onHttpWait();