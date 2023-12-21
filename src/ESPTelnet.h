/////////////////////////////////////////////////////////////////

#pragma once
#ifndef ESPTelnet_h
#define ESPTelnet_h

/////////////////////////////////////////////////////////////////

#include "ESPTelnetBase.h"

/////////////////////////////////////////////////////////////////

class ESPTelnet : public ESPTelnetBase {
 public:
  using ESPTelnetBase::ESPTelnetBase;

  
  template<typename T>
  void print(const T& data) {
    if (client && isConnected()) {
      if (!client.print(data)) {
        this->disconnectClient();
      }
    }
  }

  template<typename T>
  void println(const T& data) {
    if (client && isConnected()) {
      if (!client.println(data)) {
        this->disconnectClient();
      }
    }
  }

  template<typename T>
  void print(const T& data, int base) {
    if (client && isConnected()) {
      if (!client.print(data, base)) {
        this->disconnectClient();
      }
    }
  }

  template<typename T>
  void println(const T& data, int base) {
    if (client && isConnected()) {
      if (!client.println(data, base)) {
        this->disconnectClient();
      }
    }
  }

  void println();
  size_t printf(const char *format, ...);

  bool isLineModeSet();
  void setLineMode(bool value = true);

 protected:
  bool _lineMode = true;

  void handleInput();
};

/////////////////////////////////////////////////////////////////

// << operator
template <class T>
inline ESPTelnet &operator<<(ESPTelnet &obj, T arg) {
  obj.print(arg);
  return obj;
}

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
