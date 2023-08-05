/////////////////////////////////////////////////////////////////
/*

  Implements a joint WiFI/Ethernet server class

*/
/////////////////////////////////////////////////////////////////

#pragma once
#ifndef TCPServer_h
#define TCPServer_h

#include <Ethernet.h>
// Ethernet defines it to be 8 and Wifi redefines it to be 4, let's use the lower
#undef MAX_SOCK_NUM 

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#endif
#include "TCPClient.h"

/////////////////////////////////////////////////////////////////

class TCPServer : public WiFiServer, EthernetServer {
  public:
    TCPServer(uint16_t port = 23, bool useEthernet = false) : WiFiServer(port), EthernetServer(port), _useEthernet(useEthernet) {};

// Methods override
    TCPClient accept();
    bool hasClient();
    void begin(uint16_t port = 0);
    void begin(uint16_t port, int reuse_enable);

// Convenience methods
    bool useEthernet(void);
    const char *connection(void);

  protected:
    bool _useEthernet;
};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
