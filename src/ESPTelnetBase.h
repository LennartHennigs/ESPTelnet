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

#include <Ethernet.h>
// Ethernet defines it to be 8 and Wifi redefines it to be 4, let's use the lower
#undef MAX_SOCK_NUM 

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#endif

/////////////////////////////////////////////////////////////////

#include "DebugMacros.h"

/////////////////////////////////////////////////////////////////

class TCPClient : public WiFiClient, EthernetClient {
  public:
    TCPClient() : _useEthernet(false) {}

// Convenience methods
    bool useEthernet(void) { return _useEthernet; }
    const char *connection(void) { return _useEthernet ? "Ethernet" : "WiFi"; }

// Methods override
    int read() { return _useEthernet ? EthernetClient::read() : WiFiClient::read(); }
    int read(uint8_t *buf, size_t size) { return _useEthernet ? EthernetClient::read(buf, size) : WiFiClient::read(buf, size); }
    size_t write(uint8_t data) { return _useEthernet ? EthernetClient::write(data) : WiFiClient::write(data); }
    size_t write(const uint8_t *buf, size_t size) { return _useEthernet ? EthernetClient::write(buf, size) : WiFiClient::write(buf, size); }
    int available() { return _useEthernet ? EthernetClient::available() : WiFiClient::available(); }
    int peek() { return _useEthernet ? EthernetClient::peek() : WiFiClient::peek(); }
    uint8_t connected() { return _useEthernet ? EthernetClient::connected() : WiFiClient::connected(); }
    IPAddress remoteIP() override { return _useEthernet ? EthernetClient::remoteIP() : WiFiClient::remoteIP(); }
    IPAddress remoteIP(int fd) { 
      return _useEthernet ? EthernetClient::remoteIP() : 
        #if defined(ARDUINO_ARCH_ESP8266)
        WiFiClient::remoteIP(); 
        #elif defined(ARDUINO_ARCH_ESP32)
        WiFiClient::remoteIP(fd); 
        #endif
    }
    void flush() { if(_useEthernet) EthernetClient::flush(); else WiFiClient::flush(); }
    void stop() { if(_useEthernet) EthernetClient::stop(); else WiFiClient::stop(); }
    void setNoDelay(bool nodelay) { if(!_useEthernet) WiFiClient::setNoDelay(nodelay); }
    uint8_t status() {
      if (_useEthernet) {
        return EthernetClient::status();
      } else {
        return WiFiClient::connected() ? WL_CONNECTED : WL_DISCONNECTED;
      }
    }

// Copy constructors and assignment operators
    TCPClient(EthernetClient ec) : EthernetClient(ec), _useEthernet(true) {}
    TCPClient(WiFiClient wc) : WiFiClient(wc), _useEthernet(false) {}
    TCPClient & operator=(const EthernetClient &other) { _useEthernet = true; return *this; }
    TCPClient & operator=(const WiFiClient &other) { _useEthernet = false; return *this; }

// Conversion operators
    operator bool() { return _useEthernet ? EthernetClient::operator bool() : WiFiClient::operator bool(); }

// Print methods to keep ESPTelnetStream class happy
    size_t print(const __FlashStringHelper *ifsh) { return _useEthernet ? EthernetClient::print(ifsh) : WiFiClient::print(ifsh); }
    size_t print(const String &s)                 { return _useEthernet ? EthernetClient::print(s)  :  WiFiClient::print(s); }
    size_t print(const char s[])                  { return _useEthernet ? EthernetClient::print(s)  :  WiFiClient::print(s); }
    size_t print(char c)                          { return _useEthernet ? EthernetClient::print(c)  :  WiFiClient::print(c); }
    size_t print(unsigned char c, int b = DEC)    { return _useEthernet ? EthernetClient::print(c, b)  :  WiFiClient::print(c, b); }
    size_t print(int c, int b = DEC)              { return _useEthernet ? EthernetClient::print(c, b)  :  WiFiClient::print(c, b); }
    size_t print(unsigned int c, int b = DEC)     { return _useEthernet ? EthernetClient::print(c, b)  :  WiFiClient::print(c, b); }
    size_t print(long l, int b= DEC)              { return _useEthernet ? EthernetClient::print(l, b)  :  WiFiClient::print(l, b); }
    size_t print(unsigned long l, int b = DEC)    { return _useEthernet ? EthernetClient::print(l, b)  :  WiFiClient::print(l, b); }
    size_t print(long long l, int b = DEC)        { return _useEthernet ? EthernetClient::print(l, b)  :  WiFiClient::print(l, b); }
    size_t print(unsigned long long l, int b = DEC) { return _useEthernet ? EthernetClient::print(l, b)  :  WiFiClient::print(l, b); }
    size_t print(double d, int i = 2)             { return _useEthernet ? EthernetClient::print(d, i)  :  WiFiClient::print(d, i); }
    size_t print(const Printable& p)              { return _useEthernet ? EthernetClient::print(p)  :  WiFiClient::print(p); }
    size_t print(struct tm * timeinfo, const char * format = NULL) { 
      char buffer[50];
      strftime(buffer, sizeof(buffer), format, timeinfo);
      return _useEthernet ? EthernetClient::print(buffer) : WiFiClient::print(buffer);      
    }
    size_t println(const __FlashStringHelper *ifsh)  { return _useEthernet ? EthernetClient::println(ifsh)  :  WiFiClient::println(ifsh); }
    size_t println(const String &s)               { return _useEthernet ? EthernetClient::println(s)  :  WiFiClient::println(s); }
    size_t println(const char s[])                { return _useEthernet ? EthernetClient::println(s)  :  WiFiClient::println(s); }
    size_t println(char c)                        { return _useEthernet ? EthernetClient::println(c)  :  WiFiClient::println(c); }
    size_t println(unsigned char c, int b = DEC)  { return _useEthernet ? EthernetClient::println(c, b)  :  WiFiClient::println(c, b); }
    size_t println(int c, int b = DEC)            { return _useEthernet ? EthernetClient::println(c, b)  :  WiFiClient::println(c, b); }
    size_t println(unsigned int c, int b = DEC)   { return _useEthernet ? EthernetClient::println(c, b)  :  WiFiClient::println(c, b); }
    size_t println(long l, int b = DEC)           { return _useEthernet ? EthernetClient::println(l, b)  :  WiFiClient::println(l, b); }
    size_t println(unsigned long l, int b = DEC)  { return _useEthernet ? EthernetClient::println(l, b)  :  WiFiClient::println(l, b); }
    size_t println(long long l, int b = DEC)      { return _useEthernet ? EthernetClient::println(l, b)  :  WiFiClient::println(l, b); }
    size_t println(unsigned long long l, int b = DEC) { return _useEthernet ? EthernetClient::println(l, b)  :  WiFiClient::println(l, b); }
    size_t println(double d, int b = 2)           { return _useEthernet ? EthernetClient::println(d, b)  :  WiFiClient::println(d, b); }
    size_t println(const Printable&p)             { return _useEthernet ? EthernetClient::println(p)  :  WiFiClient::println(p); }
    size_t println(struct tm * timeinfo, const char * format = NULL) { 
      char buffer[50];
      strftime(buffer, sizeof(buffer), format, timeinfo);
      return _useEthernet ? EthernetClient::println(buffer) : WiFiClient::println(buffer);
    }
    size_t println(void)                         { return _useEthernet ? EthernetClient::println()  :  WiFiClient::println(); }

  protected:
    bool _useEthernet;
};

/////////////////////////////////////////////////////////////////

class TCPServer : public WiFiServer, EthernetServer {
  public:
    TCPServer(uint16_t port, bool useEthernet = false) : WiFiServer(port), EthernetServer(port), _useEthernet(useEthernet) {}

// Methods override
    TCPClient accept() {if(_useEthernet) return EthernetServer::accept(); else return WiFiServer::accept(); }
    bool hasClient() { return _useEthernet ? true : WiFiServer::hasClient(); }

    void begin(uint16_t port=0) {if(_useEthernet) EthernetServer::begin(); else WiFiServer::begin(port); }
    void begin(uint16_t port, int reuse_enable) {if(_useEthernet) EthernetServer::begin(); else WiFiServer::begin(port, reuse_enable); }

// Convenience methods
    bool useEthernet(void) { return _useEthernet; }
    const char *connection(void) { return _useEthernet ? "Ethernet" : "WiFi";}

  protected:
    bool _useEthernet;
};


/////////////////////////////////////////////////////////////////

class ESPTelnetBase {
  typedef void (*CallbackFunction)(String str);

 public:
  ESPTelnetBase();

  bool begin(uint16_t port = 23, bool checkWiFiConnection = true, bool useEthernet = false);
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
  TCPServer server; // = TCPServer(23);  // must be initalized here  ==> WHY ?
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
