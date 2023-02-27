/////////////////////////////////////////////////////////////////

#include "ESPTelnetBase.h"

/////////////////////////////////////////////////////////////////

ESPTelnetBase::ESPTelnetBase() {
  connected = false;
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
    if (!isConnected()) {
      connectClient();
    } else {
      if (!isConnected()) {
        disconnectClient();
        return;
      }
      TCPClient newClient = server.accept();
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
  }
  // frequently check if client is still alive
  if (doKeepAliveCheckNow() && !isConnected()) {
    disconnectClient();
  }
  // check for input
  if (on_input != NULL && client && client.available()) {
    handleInput();
  }
  yield();
}

/////////////////////////////////////////////////////////////////

bool ESPTelnetBase::doKeepAliveCheckNow() {
  long now = millis();
  if (now - last_status_check >= keep_alive_interval) {
    last_status_check = now;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::setKeepAliveInterval(int interval) {
  keep_alive_interval = interval;
}

/////////////////////////////////////////////////////////////////

int ESPTelnetBase::getKeepAliveInterval() {
  return keep_alive_interval;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::connectClient(bool triggerEvent) {
  client = server.accept();
  ip = client.remoteIP().toString();
  client.setNoDelay(true);
  if (triggerEvent && on_connect != NULL) on_connect(ip);
  emptyClientStream();
  connected = true;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::disconnectClient(bool triggerEvent) {
  if (connected) {
    emptyClientStream();
    client.stop();
    if (triggerEvent && on_disconnect != NULL) on_disconnect(ip);
    ip = "";
    connected = false;
  }
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

bool ESPTelnetBase::isConnected() {
  bool res;
#if defined(ARDUINO_ARCH_ESP8266)
  res = client.status() == ESTABLISHED;
#elif defined(ARDUINO_ARCH_ESP32)
  res = client.connected();
#endif
  return res;
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
