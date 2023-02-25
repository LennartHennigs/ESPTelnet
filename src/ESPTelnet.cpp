/* ------------------------------------------------- */

#include "ESPTelnet.h"

/* ------------------------------------------------- */

// !!!
#define SHOW_ASCII true

// http://pcmicro.com/netfoss/telnet.html
#define TELNET_IAC 0xff   // 255
#define TELNET_DONT 0xfe  // 254
#define TELNET_DO 0xfd    // 253
#define TELNET_WONT 0xfc  // 252
#define TELNET_WILL 0xfb  // 251

#define TELNET_OPT_ECHO 1    // https://www.rfc-editor.org/rfc/rfc857
#define TELNET_OPT_TIMING 6  // https://www.rfc-editor.org/rfc/rfc860
/* ------------------------------------------------- */

void ESPTelnet::manageRequest() {
  char cmd = client.read();
  char opt = client.read();
  Serial.print(int(cmd));
  Serial.print(" - ");
  Serial.println(int(opt));
  switch (cmd) {
    case TELNET_DO:
      this->print(TELNET_IAC);
      this->print(TELNET_WONT);
      this->print(opt);
      break;

    case TELNET_DONT:
      break;
  }
}

/* ------------------------------------------------- */

void ESPTelnet::handleInput() {
  char c = client.read();
  // !!!
  if (c == TELNET_IAC) {
    manageRequest();
    return;
  }
  // collect string
  if (_lineMode) {
    // EOL -> send input
    if (c == '\n') {
      on_input(input);
      input = "";
      // is it valid ASCII?
    } else if (c >= 32 && c < 127) {
      input += c;
    }
    // send individual characters
  } else {
    if (input.length()) {
      on_input(input + String(c));
      input = "";
    } else {
      if (SHOW_ASCII && (c < 32 || c > 127)) {
        if (c != 13 && c != 10)
          on_input(String("[") + String(c) + String("]"));
      } else {
        on_input(String(c));
      }
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

void ESPTelnet::print(const String& str) {
  if (client && isClientConnected(client)) {
    client.print(str);
  }
}

/* ------------------------------------------------- */

void ESPTelnet::println(const String& str) {
  client.println(str);
}

/* ------------------------------------------------- */

void ESPTelnet::println(const char c) {
  client.println(c);
}

/* ------------------------------------------------- */

void ESPTelnet::println() {
  client.println();
}

/* ------------------------------------------------- */

void ESPTelnet::print(unsigned char b, int base) {
  client.print(b, base);
}

/* ------------------------------------------------- */

void ESPTelnet::println(unsigned char b, int base) {
  client.println(b, base);
}

/* ------------------------------------------------- */

void ESPTelnet::print(const Printable& x) {
  client.print(x);
}

/* ------------------------------------------------- */

void ESPTelnet::println(const Printable& x) {
  client.println(x);
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
