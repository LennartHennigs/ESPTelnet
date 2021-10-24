/* ------------------------------------------------- */

#include "ESPTelnet.h"

/* ------------------------------------------------- */

ESPTelnet::ESPTelnet() : server(23),
                         isConnected(false),
                         ip(""),
                         attemptIp("") {
}

/* ------------------------------------------------- */

bool ESPTelnet::begin() {
  ip = "";
  // connected to WiFi or is ESP in AP mode?
  if (WiFi.status() == WL_CONNECTED /* || WiFi.softAPIP().isSet()*/) {
    server.begin();
    server.setNoDelay(true);
    return true;
  }

  return false;
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

void ESPTelnet::loop() {
  // check if there are any new clients
  if (server.hasClient()) {
    isConnected = true;
    // already a connection?
    if (client && client.connected() && isClientConnected(client)) {
      WiFiClient newClient = server.available();
      attemptIp            = newClient.remoteIP().toString();
      // reconnected?
      if (attemptIp == ip) {
        if (on_reconnect != NULL)
          on_reconnect(ip);
        client.stop();
        client = newClient;
        // disconnect the second connection
      } else {
        if (on_connection_attempt != NULL)
          on_connection_attempt(ip);
        return;
      }
      // first connection
    } else {
      client = server.available();
      ip     = client.remoteIP().toString();
      if (on_connect != NULL)
        on_connect(ip);
      client.setNoDelay(true);
      client.flush();
    }
  }
  // check whether to disconnect
  if (client && isConnected && !isClientConnected(client)) {
    if (on_disconnect != NULL)
      on_disconnect(ip);
    isConnected = false;
    ip          = "";
  }
  yield();
}

/* ------------------------------------------------- */

int ESPTelnet::available() {
  if (client && isClientConnected(client)) {
    return client.available();
  }

  return 0;
}

/* ------------------------------------------------- */

int ESPTelnet::read() {
  if (client && isClientConnected(client)) {
    return client.read();
  }

  return 0;
}

/* ------------------------------------------------- */

int ESPTelnet::peek() {
  if (client && isClientConnected(client)) {
    return client.peek();
  }

  return 0;
}

/* ------------------------------------------------- */

void ESPTelnet::flush() {
  if (client && isClientConnected(client)) {
    client.flush();
  }
}

/* ------------------------------------------------- */

size_t ESPTelnet::write(uint8_t data) {
  if (client && isClientConnected(client)) {
    return client.write(data);
  }

  return 0;
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

void ESPTelnet::onConnectionAttempt(CallbackFunction f) {
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
