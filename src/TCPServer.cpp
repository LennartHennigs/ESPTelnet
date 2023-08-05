/////////////////////////////////////////////////////////////////

#include "TCPServer.h"

/////////////////////////////////////////////////////////////////


TCPClient TCPServer::accept() {
    if (_useEthernet) {
        return EthernetServer::accept();
    } else {
        return WiFiServer::accept(); 
    }
}

/////////////////////////////////////////////////////////////////

bool TCPServer::hasClient() {
    return (_useEthernet) ? true : WiFiServer::hasClient(); 
}

/////////////////////////////////////////////////////////////////

void TCPServer::begin(uint16_t port /* = 0 */) {
    if(_useEthernet) {
        EthernetServer::begin(); 
    } else {
        WiFiServer::begin(port); 
    }
}

/////////////////////////////////////////////////////////////////

void TCPServer::begin(uint16_t port, int reuse_enable) {
    if(_useEthernet) {
        EthernetServer::begin();
        } else {
        WiFiServer::begin(port, reuse_enable); 
    }
}

/////////////////////////////////////////////////////////////////

bool TCPServer::useEthernet(void) { 
    return _useEthernet; 
}

/////////////////////////////////////////////////////////////////

const char* TCPServer::connection(void) { 
    return (_useEthernet) ? "Ethernet" : "WiFi";
}

/////////////////////////////////////////////////////////////////
