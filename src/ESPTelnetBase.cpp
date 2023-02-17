/////////////////////////////////////////////////////////////////

#include "ESPTelnetBase.h"

/////////////////////////////////////////////////////////////////

ESPTelnetBase::ESPTelnetBase() {
  isConnected = false;
}

/////////////////////////////////////////////////////////////////

bool ESPTelnetBase::begin(uint16_t port /* = 23 */, bool checkConnection /* = true */) {
  ip = "";
  if (checkConnection) {
    // connected to WiFi or is ESP in AP mode?
    if (WiFi.status() != WL_CONNECTED && !_isIPSet(WiFi.softAPIP())) return false;
  }
  server_port = port;
  server = TCPServer(port);
  server.begin();
  server.setNoDelay(true);
  return true;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::loop() {
  // is there a new client wating?
  if (server.hasClient()) {
    // no exisintg connection?
    if (!isConnected) {
      connectClient();
      // check if this a reconnection attempt
    } else {
      if (!isClientConnected(client)) {
        disconnectClient();
        return;
      }

      TCPClient newClient = server.available();
      attemptIp = newClient.remoteIP().toString();
      // yes, reconnected
      if (attemptIp == ip) {
        disconnectClient(false);
        connectClient(false);
        if (on_reconnect != NULL) on_reconnect(attemptIp);
        // no, throw error
      } else {
        if (on_connection_attempt != NULL) on_connection_attempt(attemptIp);
        return;
      }
    }

    // handle input
    if (on_input != NULL && client && client.available()) {
      handleInput();
    }
    yield();
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::connectClient(bool triggerEvent) {
  client = server.available();
  ip = client.remoteIP().toString();
  isConnected = true;
  client.setNoDelay(true);
  if (triggerEvent && on_connect != NULL) on_connect(ip);
  emptyClientStream();
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::disconnectClient(bool triggerEvent) {
  emptyClientStream();
  client.stop();
  if (triggerEvent && on_disconnect != NULL) on_disconnect(ip);
  ip = "";
  isConnected = false;
}

/////////////////////////////////////////////////////////////////
// helper method, as ESP32's IPAddress has no isSet() method

bool ESPTelnetBase::_isIPSet(IPAddress ip) {
#if defined(ARDUINO_ARCH_ESP8266)
  return ip.isSet();
#else
  // this works for ESP32, hopefully for others as well
  return ip.toString() != "0.0.0.0";
#endif
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::stop() {
  server.stop();
}

/////////////////////////////////////////////////////////////////

bool ESPTelnetBase::isClientConnected(TCPClient &client) {
#if defined(ARDUINO_ARCH_ESP8266)
  return client.status() == ESTABLISHED;
#elif defined(ARDUINO_ARCH_ESP32)
  return client.connected();
#endif
}

/////////////////////////////////////////////////////////////////

String ESPTelnetBase::getIP() const {
  return ip;
}

/////////////////////////////////////////////////////////////////

String ESPTelnetBase::getLastAttemptIP() const {
  return attemptIp;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::emptyClientStream() {
  client.flush();
  delay(50);
  while (client.available()) {
    client.read();
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onConnect(CallbackFunction f) {
  on_connect = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onConnectionAttempt(CallbackFunction f) {
  on_connection_attempt = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onReconnect(CallbackFunction f) {
  on_reconnect = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onDisconnect(CallbackFunction f) {
  on_disconnect = f;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onInputReceived(CallbackFunction f) {
  on_input = f;
}

/////////////////////////////////////////////////////////////////
