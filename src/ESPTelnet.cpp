/////////////////////////////////////////////////////////////////

#include "ESPTelnet.h"

/////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////

void ESPTelnet::println() {
  if (client && isConnected()) {
    client.println();
  }
}

/////////////////////////////////////////////////////////////////

size_t ESPTelnet::printf(const char* format, ...) {
  int len = 0;
  if (client && isConnected()) {
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