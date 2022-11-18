#include "ESPTelnetBase.h"

/* ------------------------------------------------- */

ESPTelnetBase::ESPTelnetBase() { 
  isConnected = false; 
}

/* ------------------------------------------------- */

bool ESPTelnetBase::begin(uint16_t port /* = 23 */, bool checkConnection /* = true */) {
  ip = "";
  // connected to WiFi or is ESP in AP mode?

  if (!checkConnection || (WiFi.status() == WL_CONNECTED || _isIPSet(WiFi.softAPIP()))) {
    server_port = port;
    server = TCPServer(port);
    server.begin();
    server.setNoDelay(true);
    return true;
  } else {
    return false;
  }
}

/* ------------------------------------------------- */

void ESPTelnetBase::loop() {
  //check if there are any new clients
  if (server.hasClient()) {
    isConnected = true;
    // already a connection?
    if (client && client.connected() && isClientConnected(client)) {
      TCPClient newClient = server.available();
      attemptIp  = newClient.remoteIP().toString();
      // reconnected?
      if (attemptIp == ip) {
        if (on_reconnect != NULL) on_reconnect(ip);
        disconnectClient();
        client = newClient;
        emptyClientStream();
      // disconnect the second connection
      } else {
        if (on_connection_attempt != NULL) on_connection_attempt(ip);
        return;
      }
    // first connection
    } else {
      client = server.available();
      ip = client.remoteIP().toString();
      if (on_connect != NULL) on_connect(ip);
      client.setNoDelay(true);
      emptyClientStream();
    }
  }
  // check whether to disconnect
  if (isConnected) {
    if(!(client || isClientConnected(client))) {
        disconnectClient();
    } 
    // gather input
    if (on_input != NULL && client && client.available()) {
        handleInput();
    }
  }
  yield();
} 

/* ------------------------------------------------- */
// helper method, as ESP32's IPAddress has no isSet() method

bool ESPTelnetBase::_isIPSet(IPAddress ip) {
#if defined(ARDUINO_ARCH_ESP8266)
  return ip.isSet();
#else
  // this works for ESP32, hopefully for others as well
  return ip.toString() != "0.0.0.0";
#endif
}

/* ------------------------------------------------- */

void ESPTelnetBase::stop() { 
  server.stop(); 
}

/* ------------------------------------------------- */

bool ESPTelnetBase::isClientConnected(TCPClient &client) {
#if defined(ARDUINO_ARCH_ESP8266)
  return client.status() == ESTABLISHED;
#elif defined(ARDUINO_ARCH_ESP32)
  return client.connected();
#endif
}

/* ------------------------------------------------- */

String ESPTelnetBase::getIP() const { 
  return ip; 
}

/* ------------------------------------------------- */

String ESPTelnetBase::getLastAttemptIP() const { 
  return attemptIp; 
}

/* ------------------------------------------------- */

void ESPTelnetBase::emptyClientStream() {
  client.flush();
  delay(50);
  while (client.available()) {
    client.read();
  }        
}

/* ------------------------------------------------- */

void ESPTelnetBase::disconnectClient() {
    emptyClientStream();
    client.stop();
    if (on_disconnect != NULL) on_disconnect(ip);
    isConnected = false;
    ip = "";
}

/* ------------------------------------------------- */

void ESPTelnetBase::onConnect(CallbackFunction f) { 
  on_connect = f; 
}

/* ------------------------------------------------- */

void ESPTelnetBase::onConnectionAttempt(CallbackFunction f)  { 
  on_connection_attempt = f; 
}

/* ------------------------------------------------- */

void ESPTelnetBase::onReconnect(CallbackFunction f) { 
  on_reconnect = f; 
}

/* ------------------------------------------------- */

void ESPTelnetBase::onDisconnect(CallbackFunction f) { 
  on_disconnect = f; 
}

/* ------------------------------------------------- */

void ESPTelnetBase::onInputReceived(CallbackFunction f) { 
  on_input = f; 
}

/* ------------------------------------------------- */
