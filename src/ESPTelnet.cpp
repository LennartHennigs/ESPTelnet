/////////////////////////////////////////////////////////////////

#include "ESPTelnet.h"

#ifndef ESPTELNET_VPRINTF_BUFFER_SIZE
#define ESPTELNET_VPRINTF_BUFFER_SIZE 64
#endif

/////////////////////////////////////////////////////////////////

void ESPTelnet::handleInput() {
  char c = client.read();
  // collect string
  if (_lineMode) {
    if (c != _newlineCharacter) {
      if (c >= ASCII_SPACE && c < ASCII_DEL) {
        if (input.length() < MAX_INPUT_LENGTH) input += c;
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
    if (!client.println()) {
      onFailedWrite();
    } else {
      onSuccessfullyWrite();
    }
  }
}

/////////////////////////////////////////////////////////////////

size_t ESPTelnet::printf(const char* format, ...) {
  if (!client || !isConnected()) return 0;
  
  va_list arg;
  va_start(arg, format);
  size_t len = vprintf(format, arg);
  va_end(arg);
  return len;
}

size_t ESPTelnet::vprintf(const char* format, va_list arg) {
  char loc_buf[ESPTELNET_VPRINTF_BUFFER_SIZE];
  int len = vsnprintf(loc_buf, sizeof(loc_buf), format, arg);
  if (len < 0) return 0;

  if (len >= (int)sizeof(loc_buf)) {
    char* temp = (char*)malloc(len + 1);
    if (temp == nullptr) {
      return 0;
    }
    vsnprintf(temp, len + 1, format, arg);
    len = write((uint8_t*)temp, len);
    free(temp);
  } else {
    len = write((uint8_t*)loc_buf, len);
  }

  return len;
}
/////////////////////////////////////////////////////////////////

bool ESPTelnet::isLineModeSet() const {
  return _lineMode;
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::setLineMode(bool value /* = true */) {
  _lineMode = value;
}

/////////////////////////////////////////////////////////////////

char ESPTelnet::getNewlineCharacter() const {
  return _newlineCharacter;
}

/////////////////////////////////////////////////////////////////

void ESPTelnet::setNewlineCharacter(char value /* = '\n' */) {
  _newlineCharacter = value;
}

/////////////////////////////////////////////////////////////////
