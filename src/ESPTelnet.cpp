/* ------------------------------------------------- */

#include "ESPTelnet.h"

/* ------------------------------------------------- */

ESPTelnet::ESPTelnet() { 
  isConnected = false; 
}

/* ------------------------------------------------- */
// helper method, as ESP32's IPAddress has no isSet() method

bool ESPTelnet::_isIPSet(IPAddress ip) {
#if defined(ARDUINO_ARCH_ESP8266)
  return ip.isSet();
#else
  // this works for ESP32, hopefully for others as well
  return ip.toString() == "0.0.0.0";
#endif
}

/* ------------------------------------------------- */

  bool ESPTelnet::begin() {
  ip = "";
  // connected to WiFi or is ESP in AP mode?
  if (WiFi.status() == WL_CONNECTED || _isIPSet(WiFi.softAPIP())) {
    server.begin();
    server.setNoDelay(true);
    return true;
  } else {
    return false;
  }
}

/* ------------------------------------------------- */

void ESPTelnet::stop() { 
  server.stop(); 
}

/* ------------------------------------------------- */

bool ESPTelnet::isClientConnected(WiFiClient client) {
#if defined(ARDUINO_ARCH_ESP8266)
  return client.status() == ESTABLISHED;
#elif defined(ARDUINO_ARCH_ESP32)
  return client.connected();
#endif
}

/* ------------------------------------------------- */

void ESPTelnet::emptyClientStream() {
  client.flush();
  delay(50);
  while (client.available()) {
    client.read();
  }        
}
/* ------------------------------------------------- */

void ESPTelnet::loop() {
  //check if there are any new clients
  if (server.hasClient()) {
    isConnected = true;
    // already a connection?
    if (client && client.connected() && isClientConnected(client)) {
      WiFiClient newClient = server.available();
      attemptIp  = newClient.remoteIP().toString();
      // reconnected?
      if (attemptIp == ip) {
        if (on_reconnect != NULL) on_reconnect(ip);
        client.stop();
        client = newClient;
        client.flush();
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
  if (isConnected && !(client || isClientConnected (client))) {
    if (on_disconnect != NULL) on_disconnect(ip);
      isConnected = false;
      ip = "";
    }
  // gather input
  if (client && isConnected && client.available()) {    
    char c = client.read();

    if (_lineMode) {
      if (c != '\n') {
        if (c >= 32 && c < 127) {
          input += c; 
        }
    
        // EOL -> send input
      } else {
        if (on_input != NULL) on_input(input);
        input = "";
      }
    } else {
      if (on_input != NULL) {
        if (input.length()) {
          on_input(input + c);
          input = "";
        } else {
          on_input(String(c));
        }
      }
    }
  }
  
  yield();
} 
  
/* ------------------------------------------------- */
    
void ESPTelnet::print(char c) {
  if (client && isClientConnected(client)) {
    client.print(c); 
  }
}

/* ------------------------------------------------- */

void ESPTelnet::print(String str) {
  if (client && isClientConnected(client)) {
    client.print(str); 
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println(String str) { 
  client.println(str); 
}

/* ------------------------------------------------- */

void ESPTelnet::println(char c) { 
  client.println(c); 
}

/* ------------------------------------------------- */

void ESPTelnet::println() { 
  client.println(); 
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

void ESPTelnet::onInputReceived(CallbackFunction f) { 
  on_input = f; 
}

/* ------------------------------------------------- */

bool ESPTelnet::lineMode() {
  return _lineMode;
}

/* ------------------------------------------------- */

void ESPTelnet::lineMode(bool value) {
  _lineMode = value;
}
