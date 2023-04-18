/////////////////////////////////////////////////////////////////

#pragma once
#ifndef ESPTelnetBase_h
#define ESPTelnetBase_h

/////////////////////////////////////////////////////////////////

#define ASCII_BELL 7
#define ASCII_BACKSPACE 8
#define ASCII_LF 10
#define ASCII_CR 13
#define KEEP_ALIVE_INTERVAL_MS 1000

/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#endif

/////////////////////////////////////////////////////////////////

#include "DebugMacros.h"

/////////////////////////////////////////////////////////////////

using TCPClient = WiFiClient;
using TCPServer = WiFiServer;

/////////////////////////////////////////////////////////////////
class ESPTelnetBase {
  typedef void (*CallbackFunction)(String str);

 public:
  ESPTelnetBase();

  bool begin(uint16_t port = 23, bool checkConnection = true);
  void stop();
  void loop();

  bool isConnected();
  void disconnectClient(bool triggerEvent = true);

  void setKeepAliveInterval(int ms);
  int getKeepAliveInterval();

  String getIP() const;
  String getLastAttemptIP() const;

  void onConnect(CallbackFunction f);
  void onConnectionAttempt(CallbackFunction f);
  void onReconnect(CallbackFunction f);
  void onDisconnect(CallbackFunction f);
  void onInputReceived(CallbackFunction f);

 protected:
  TCPServer server = TCPServer(23);  // must be initalized here
  TCPClient client;
  bool connected = false;  // needed because I cannot do "client = NULL"
  String ip = "";
  String attemptIp;
  String input = "";

  uint16_t server_port = 23;
  int keep_alive_interval = KEEP_ALIVE_INTERVAL_MS;
  long last_status_check;

  CallbackFunction on_connect = NULL;
  CallbackFunction on_reconnect = NULL;
  CallbackFunction on_disconnect = NULL;
  CallbackFunction on_connection_attempt = NULL;
  CallbackFunction on_input = NULL;

  void emptyClientStream();
  bool _isIPSet(IPAddress ip);
  virtual void handleInput() = 0;

 private:
  void connectClient(TCPClient c, bool triggerEvent = true);
  void processClientConnection();
  void performKeepAliveCheck();
  void handleClientInput();
  void notifyConnectionAttempt(const String &attemptedIp);
  void handleReconnection(TCPClient &newClient, const String &attemptedIp); 
  void handleExistingConnection(TCPClient &newClient);
  
  bool doKeepAliveCheckNow();

};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
