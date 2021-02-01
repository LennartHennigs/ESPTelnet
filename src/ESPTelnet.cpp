/* ------------------------------------------------- */

#include "ESPTelnet.h"

/* ------------------------------------------------- */

  bool ESPTelnet::begin() {
  if (WiFi.status() == WL_CONNECTED) {
    server.begin();
    server.setNoDelay(true);
    return true;
  } else {
    return false;
  }
}
/* ------------------------------------------------- */
void ESPTelnet::loop() {
  //check if there are any new clients
  if (server.hasClient()) {
    isConnected = true;
    // already a connection?
    if (client && client.connected() && client.status() == ESTABLISHED) {
      WiFiClient newClient = server.available();
      attemptIp  = newClient.remoteIP().toString();
      // reconnected?
      if (attemptIp == ip) {
        if (on_reconnect != NULL) on_reconnect();
        client.stop();
        client = newClient;
      // disconnect the second connection
      } else {
        if (on_connection_attempt != NULL) on_connection_attempt();
        return;
      }
    // first connection
    } else {
      client = server.available();
      ip = client.remoteIP().toString();
      if (on_connect != NULL) on_connect();
      client.setNoDelay(true);
      client.flush();
    }
  }
  // check whether to disconnect
  if (client && isConnected && client.status() == CLOSED) {
      if (on_disconnect != NULL) on_disconnect();
      isConnected = false;
      ip = "";
    }
    yield();
  } 
/* ------------------------------------------------- */

void ESPTelnet::println(String str) { 
  client.print(str + "\n"); 
}

/* ------------------------------------------------- */
    
void ESPTelnet::print(char c) {
  if (client && client.status() == ESTABLISHED) {
    client.print(c); 
  }
}

/* ------------------------------------------------- */

void ESPTelnet::print(String str) {
  if (client && client.status() == ESTABLISHED) {
    client.print(str); 
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println(char c) { 
  client.print(c + "\n"); 
}

/* ------------------------------------------------- */

void ESPTelnet::println() { 
  client.print("\n"); 
}

/* ------------------------------------------------- */
