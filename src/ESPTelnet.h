/* ------------------------------------------------- */

#pragma once

#ifndef ESPTelnet_h
#define ESPTelnet_h

/* ------------------------------------------------- */

#if defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

#include "DebugMacros.h"

/* ------------------------------------------------- */

class ESPTelnet {
  typedef void (*CallbackFunction) (String str);

  public:
    ESPTelnet();

    bool begin(uint16_t port = 23);
    void loop();
    void stop();

    void print(const String &str);
    void print(const char c);
    void println(const String &str);
    void println(const char c);
    void println();

    void print(unsigned char b, int base);
    void println(unsigned char b, int base);
    void print(const Printable& x);
    void println(const Printable& x);

    bool isLineModeSet();
    void setLineMode(bool value);

    String getIP() const;
    String getLastAttemptIP() const;
    
    void onConnect(CallbackFunction f);
    void onConnectionAttempt(CallbackFunction f);
    void onReconnect(CallbackFunction f);
    void onDisconnect(CallbackFunction f);
    void onInputReceived(CallbackFunction f);

    void disconnectClient();

  protected:
    WiFiServer server = WiFiServer(23);
    WiFiClient client;
    boolean isConnected = false;
    String ip = "";
    String attemptIp;
    String input = "";
    uint16_t server_port = 23;
    bool _lineMode = true;

    bool isClientConnected(WiFiClient &client);
    void emptyClientStream();

    CallbackFunction on_connect = NULL;
    CallbackFunction on_reconnect  = NULL;
    CallbackFunction on_disconnect = NULL;
    CallbackFunction on_connection_attempt = NULL;
    CallbackFunction on_input  = NULL;

  private:
    // for ESP32 compability
    bool _isIPSet(IPAddress ip);
};

/* ------------------------------------------------- */

  // << operator
  template<class T> inline ESPTelnet &operator <<(ESPTelnet &obj, T arg) { obj.print(arg); return obj; } 

/* ------------------------------------------------- */
#endif
/* ------------------------------------------------- */