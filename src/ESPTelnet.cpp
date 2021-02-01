/* ------------------------------------------------- */

#include "ESPTelnet.h"

/* ------------------------------------------------- */

ESPTelnet::ESPTelnet() { 
  isConnected = false; 
}

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

String ESPTelnet::getIP() const { 
  return ip; 
}

/* ------------------------------------------------- */

String ESPTelnet::getLastAttemptIP() const { 
  return attemptIp; 
}

/* ------------------------------------------------- */

void ESPTelnet::onConnect(CallbackFunction f) { 
  on_connect = f; 
}

/* ------------------------------------------------- */

void ESPTelnet::onConnectionAttempt(CallbackFunction f)  { 
  on_connection_attempt = f; 
}

/* ------------------------------------------------- */

void ESPTelnet::onReconnect(CallbackFunction f) { 
  on_reconnect = f; 
}

/* ------------------------------------------------- */

void ESPTelnet::onDisconnect(CallbackFunction f) { 
  on_disconnect = f; 
}

/* ------------------------------------------------- */

void ESPTelnet::stop() { 
  server.stop(); 
}

/* ------------------------------------------------- */