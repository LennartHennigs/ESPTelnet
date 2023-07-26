/////////////////////////////////////////////////////////////////
/*
  
  Implements a joint WiFI/Ethernet server class

*/
/////////////////////////////////////////////////////////////////

#pragma once
#ifndef TCPServer_h
#define TCPServer_h

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
#endif
/////////////////////////////////////////////////////////////////
