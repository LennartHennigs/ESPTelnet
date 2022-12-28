/* ------------------------------------------------- */

#include "ESPTelnet.h"

/* ------------------------------------------------- */

void ESPTelnet::handleInput() {
  char c = client.read();
  // collect string
  if (_lineMode) {
    if (c != '\n') {
      if (c >= 32 && c < 127) {
        input += c; 
      }
    // EOL -> send input
    } else {
      on_input(input);
      input = "";
    }
    
  // send individual characters
  } else {
    if (input.length()) {
      on_input(input + String(c));
      input = "";
    } else {
      on_input(String(c));
    }
  }  
}

/* ------------------------------------------------- */
    
void ESPTelnet::print(const char c) {
  if (client && isClientConnected(client)) {
    client.print(c); 
  }
}

/* ------------------------------------------------- */

void ESPTelnet::print(const String &str) {
  if (client && isClientConnected(client)) {
    client.print(str); 
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println(const String &str) {
  if (client && isClientConnected(client)) {
    client.println(str);
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println(const char c) {
  if (client && isClientConnected(client)) {
    client.println(c);
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println() {
  if (client && isClientConnected(client)) {
    client.println();
  }
}

/* ------------------------------------------------- */

void ESPTelnet::print(unsigned char b, int base) {
  if (client && isClientConnected(client)) {
    client.print(b,base);
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println(unsigned char b, int base) {
  if (client && isClientConnected(client)) {
    client.println(b,base);
  }
}

/* ------------------------------------------------- */

void ESPTelnet::print(const Printable& x) {
  if (client && isClientConnected(client)) {
    client.print(x);
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println(const Printable& x) {
  if (client && isClientConnected(client)) {
    client.println(x);
  }
}

/* ------------------------------------------------- */

void ESPTelnet::printf(const char *format, ...) {
  if (client && isClientConnected(client)) {
    va_list argptr;
    va_start(argptr, format);

    client.printf(format, argptr); 

    va_end(argptr);
  }
}

/* ------------------------------------------------- */

bool ESPTelnet::isLineModeSet() {
  return _lineMode;
}

/* ------------------------------------------------- */

void ESPTelnet::setLineMode(bool value /* = true */) {
  _lineMode = value;
}

/* ------------------------------------------------- */
