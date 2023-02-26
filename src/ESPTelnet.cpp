/////////////////////////////////////////////////////////////////

#include "ESPTelnet.h"

/////////////////////////////////////////////////////////////////

// !!!
#define SHOW_ASCII true

// http://pcmicro.com/netfoss/telnet.html
#define TELNET_IAC 255
#define TELNET_DONT 254
#define TELNET_DO 253
#define TELNET_WONT 252
#define TELNET_WILL 251
#define TELNET_AYT 246 
#define TELNET_AYT_ANSWER "YES!"

#define TELNET_OPT_ECHO 1    // https://www.rfc-editor.org/rfc/rfc857
#define TELNET_OPT_BINARY 2    // https://www.rfc-editor.org/rfc/rfc857
#define TELNET_OPT_SUPPRESS_GO_AHEAD 3  // http://pcmicro.com/netfoss/RFC858.html
#define TELNET_OPT_STATUS 5  // http://pcmicro.com/netfoss/RFC858.html
#define TELNET_OPT_TIMING_MARK 6  // https://www.rfc-editor.org/rfc/rfc860
#define TELNET_OPT_TIMING_MARK 6  // https://www.rfc-editor.org/rfc/rfc860

/////////////////////////////////////////////////////////////////

bool isValidCommand(char c) {
  return (c == TELNET_DONT || c == TELNET_DO || c == TELNET_WILL || c == TELNET_WONT || c == TELNET_AYT);
}

/////////////////////////////////////////////////////////////////

String cmd2str(int c) {
  if (c == TELNET_DONT) return "DONT";
  if (c == TELNET_WONT) return "WONT";
  if (c == TELNET_DO) return "DO";
  if (c == TELNET_WILL) return "WILL";
  if (c == TELNET_AYT) return "AYT";

  return "?";
}
/////////////////////////////////////////////////////////////////


void ESPTelnet::handleInput() {
  char c = client.read();

  if (c == TELNET_IAC) {
    // read command
    char cmd = client.read();
    if (isValidCommand(cmd)) {
      Serial.print(cmd2str(cmd));
      if (cmd == 246) {
        Serial.println();
        client.println(TELNET_AYT_ANSWER);
        return;
      }
      // read the option
      char option = client.read();
      Serial.print(" ");
      Serial.println(option, DEC);
//      on_telnet_option(cmd, option);
    } else {
      Serial.print("invalid command: ");
      Serial.println(cmd, DEC);
    } 
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

/////////////////////////////////////////////////////////////////

void ESPTelnet::print(const char c) {
  if (client && isClientConnected(client)) {
    client.print(c);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::print(const String& str) {
  if (client && isClientConnected(client)) {
    client.print(str);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::println(const String& str) {
  if (client && isClientConnected(client)) {
    client.println(str);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::println(const char c) {
  if (client && isClientConnected(client)) {
    client.println(c);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::println() {
  if (client && isClientConnected(client)) {
    client.println();
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::print(unsigned char b, int base) {
  if (client && isClientConnected(client)) {
    client.print(b, base);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::println(unsigned char b, int base) {
  if (client && isClientConnected(client)) {
    client.println(b, base);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::print(const Printable& x) {
  if (client && isClientConnected(client)) {
    client.print(x);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::println(const Printable& x) {
  if (client && isClientConnected(client)) {
    client.println(x);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::print(unsigned int n, int base) {
  if (client && isClientConnected(client)) {
    client.print(n, base);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::println(unsigned int n, int base) {
  if (client && isClientConnected(client)) {
    client.println(n, base);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::print(int n, int base) {
  if (client && isClientConnected(client)) {
    client.print(n, base);
  }
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::println(int n, int base) {
  if (client && isClientConnected(client)) {
    client.println(n, base);
  }
}

/////////////////////////////////////////////////////////////////

size_t ESPTelnet::printf(const char* format, ...) {
  int len = 0;
  if (client && isClientConnected(client)) {
    char loc_buf[64];
    char* temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if (len < 0) {
      va_end(arg);
      return 0;
    };
    if (len >= (int)sizeof(loc_buf)) {
      temp = (char*)malloc(len + 1);
      if (temp == NULL) {
        va_end(arg);
        return 0;
      }
      len = vsnprintf(temp, len + 1, format, arg);
    }
    va_end(arg);
    len = client.write((uint8_t*)temp, len);
    if (temp != loc_buf) {
      free(temp);
    }
  }
  return len;
}

/////////////////////////////////////////////////////////////////

bool ESPTelnet::isLineModeSet() {
  return _lineMode;
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::setLineMode(bool value /* = true */) {
  _lineMode = value;
}

/////////////////////////////////////////////////////////////////