/////////////////////////////////////////////////////////////////

#include "ESPTelnetStream.h"

/////////////////////////////////////////////////////////////////

void ESPTelnetStream::handleInput() {
  char c = client.read();
  on_input(String(c));
}

/////////////////////////////////////////////////////////////////

int ESPTelnetStream::available() {
  if (client && isConnected()) {
    return client.available();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

int ESPTelnetStream::read() {
  if (client && isConnected()) {
    return client.read();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

int ESPTelnetStream::peek() {
  if (client && isConnected()) {
    return client.peek();
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnetStream::flush() {
  if (client && isConnected()) {
#ifdef ARDUINO_ARCH_ESP8266
    if (!client.flush(this->getKeepAliveInterval())) {
#else
    if (!client.flush()) {
#endif
      this->disconnectClient();
    }
  }
}

/////////////////////////////////////////////////////////////////

size_t ESPTelnetStream::write(uint8_t data) {
  if (client && isConnected()) {
    size_t written = client.write(data);
    if (!written) {
      this->disconnectClient();
    }

    return written;
  } else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////