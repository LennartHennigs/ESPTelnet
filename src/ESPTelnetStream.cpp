/* ------------------------------------------------- */

#include "ESPTelnetStream.h"

/* ------------------------------------------------- */

 void ESPTelnetStream::handleInput() {
  char c = client.read();
  on_input(String(c));
}
  
/* ------------------------------------------------- */

int ESPTelnetStream::available() {
  if (client && isClientConnected(client)) {
    return client.available(); 
  } else {
    return 0;
  }
}

/* ------------------------------------------------- */

int ESPTelnetStream::read() {
  if (client && isClientConnected(client)) {
    return client.read(); 
  } else {
    return 0;
  }
}

/* ------------------------------------------------- */

int ESPTelnetStream::peek() {
  if (client && isClientConnected(client)) {
    return client.peek(); 
  } else {
    return 0;
  }
}

/* ------------------------------------------------- */

void ESPTelnetStream::flush() {
  if (client && isClientConnected(client)) {
    client.flush(); 
  }
}

/* ------------------------------------------------- */

size_t ESPTelnetStream::write(uint8_t data) {
  if (client && isClientConnected(client)) {
    return client.write(data); 
  } else {
    return 0;
  }
}

/* ------------------------------------------------- */
