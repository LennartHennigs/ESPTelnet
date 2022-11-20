/* ------------------------------------------------- */

#pragma once
#ifndef ESPTelnetBase_h
#define ESPTelnetBase_h

/* ------------------------------------------------- */

#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

using TCPClient = WiFiClient;
using TCPServer = WiFiServer;

#include "DebugMacros.h"

/* ------------------------------------------------- */

class ESPTelnetBase {
  typedef void (*CallbackFunction) (String str);

  public:
    ESPTelnetBase();

    bool begin(uint16_t port = 23, bool checkConnection = true);
    void stop();
    void loop();

    bool isClientConnected(TCPClient &client);
    void disconnectClient();

    String getIP() const;
    String getLastAttemptIP() const;

    void onConnect(CallbackFunction f);
    void onConnectionAttempt(CallbackFunction f);
    void onReconnect(CallbackFunction f);
    void onDisconnect(CallbackFunction f);
    void onInputReceived(CallbackFunction f);

    protected:
      // must be initalized here
      TCPServer server = TCPServer(23);
      TCPClient client;
      boolean isConnected = false;
      String ip = "";
      String attemptIp;
      String input = "";
      uint16_t server_port = 23;

      CallbackFunction on_connect = NULL;
      CallbackFunction on_reconnect  = NULL;
      CallbackFunction on_disconnect = NULL;
      CallbackFunction on_connection_attempt = NULL;
      CallbackFunction on_input  = NULL;
  
      void emptyClientStream();
      bool _isIPSet(IPAddress ip);
      virtual void handleInput() = 0;        
};

/* ------------------------------------------------- */
#endif
/* ------------------------------------------------- */
