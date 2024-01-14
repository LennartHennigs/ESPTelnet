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
  processClientConnection();
  performKeepAliveCheck();
  handleClientInput();
  yield();
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::processClientConnection() {
  if (!server.hasClient()) return;
  TCPClient newClient = server.accept();
  if (!connected) {
    connectClient(newClient);
  } else {
    handleExistingConnection(newClient);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::handleExistingConnection(TCPClient &newClient) {
  String attemptedIp = newClient.remoteIP().toString();

  if (!isConnected()) {
    disconnectClient();
    return;
  }

  if (attemptedIp == ip) {
    handleReconnection(newClient, attemptedIp);
  } else {
    notifyConnectionAttempt(attemptedIp);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::handleReconnection(TCPClient &newClient, const String &attemptedIp) {
  disconnectClient(false);
  connectClient(newClient, false);
  if (on_reconnect) on_reconnect(attemptedIp);
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::notifyConnectionAttempt(const String &attemptedIp) {
  if (on_connection_attempt) on_connection_attempt(attemptedIp);
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::performKeepAliveCheck() {
  if (doKeepAliveCheckNow() && connected && !isConnected()) {
    disconnectClient();
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::handleClientInput() {
  if (on_input && client && client.available()) {
    handleInput();
  }
}


/////////////////////////////////////////////////////////////////

bool ESPTelnetBase::doKeepAliveCheckNow() {
  unsigned long currentTime = millis();
  unsigned long timeElapsed = currentTime - last_status_check;

  if (timeElapsed >= static_cast<unsigned long>(keep_alive_interval)) {
    last_status_check = currentTime;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::setKeepAliveInterval(int ms) {
  keep_alive_interval = ms;
}

/////////////////////////////////////////////////////////////////

int ESPTelnetBase::getKeepAliveInterval() {
  return keep_alive_interval;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::flush() {
  if (!client || !isConnected()) {
    return;
  }

// only the ESP8266 has a "bool flush()" method
// https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiClient.cpp#L306

#ifdef ARDUINO_ARCH_ESP8266
  if (!client.flush(this->getKeepAliveInterval())) {
    onFailedWrite();
  } else {
    onSuccessfullyWrite();
  }
#else
  client.flush();
#endif
}

/////////////////////////////////////////////////////////////////

size_t ESPTelnetBase::write(uint8_t data) {
  if (!client || !isConnected()) {
    return 0;
  }

  size_t written = client.write(data);
  if (!written) {
    onFailedWrite();
  } else {
    onSuccessfullyWrite();
  }

  return written;
}

/////////////////////////////////////////////////////////////////

size_t ESPTelnetBase::write(const uint8_t* data, size_t size) {
  if (!client || !isConnected()) {
    return 0;
  }

  size_t written = client.write(data, size);
  if (written != size) {
    onFailedWrite();
  } else {
    onSuccessfullyWrite();
  }

  return written;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::connectClient(WiFiClient c, bool triggerEvent) {
  client = c;
  ip = client.remoteIP().toString();
  client.setNoDelay(true);
  client.setTimeout(this->getKeepAliveInterval());
  if (triggerEvent && on_connect != NULL) on_connect(ip);
  emptyClientStream();
  connected = true;
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::disconnectClient(bool triggerEvent) {
  emptyClientStream();
  client.stop();
  if (triggerEvent && on_disconnect != NULL) on_disconnect(ip);
  ip = "";
  connected = false;
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

void ESPTelnetBase::onFailedWrite() {
  failedWrites++;

  if (failedWrites >= MAX_ERRORS_ON_WRITE) {
    failedWrites = 0;
    disconnectClient();
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::onSuccessfullyWrite() {
  if (failedWrites > 0) {
    failedWrites = 0;
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetBase::emptyClientStream() {
  flush();
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

  TCPClient& ESPTelnetBase::getClient() {
    return client;
  }