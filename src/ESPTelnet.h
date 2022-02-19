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

    bool begin();
    void loop();
    void stop();

    void print(String str);
    void print(char c);
    void println(String str);
    void println(char c);
    void println();

    bool lineMode();
    void lineMode(bool value);

    String getIP() const;
    String getLastAttemptIP() const;
    
    void onConnect(CallbackFunction f);
    void onConnectionAttempt(CallbackFunction f);
    void onReconnect(CallbackFunction f);
    void onDisconnect(CallbackFunction f);
    void onInputReceived(CallbackFunction f);
    
  protected:
    WiFiServer server = WiFiServer(23);
    WiFiClient client;
    boolean isConnected = false;
    String ip = "";
    String attemptIp;
    String input = "";
    bool _lineMode = true;

    bool isClientConnected(WiFiClient client);
    void emptyClientStream();

    CallbackFunction on_connect = NULL;
    CallbackFunction on_reconnect  = NULL;
    CallbackFunction on_disconnect = NULL;
    CallbackFunction on_connection_attempt = NULL;
    CallbackFunction on_input  = NULL;

  private:
    bool _isIPSet(IPAddress ip);
};

/* ------------------------------------------------- */

  // << operator
  template<class T> inline ESPTelnet &operator <<(ESPTelnet &obj, T arg) { obj.print(arg); return obj; } 

/* ------------------------------------------------- */
#endif
/* ------------------------------------------------- */