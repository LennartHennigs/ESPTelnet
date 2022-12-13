/* ------------------------------------------------- */

#pragma once
#ifndef ESPTelnet_h
#define ESPTelnet_h

#define DEC 10

/* ------------------------------------------------- */

#include "ESPTelnetBase.h"

/* ------------------------------------------------- */

class ESPTelnet : public ESPTelnetBase {
  public:
    using ESPTelnetBase::ESPTelnetBase;

    void print(const String &str);
    void println(const String &str);
    void print(const char c);
    void println(const char c);
    void print(unsigned char b, int base = DEC);
    void println(unsigned char b, int base = DEC);
    void print(int n, int base);
    void println(int n, int base);
    void print(const Printable& x);
    void println(const Printable& x);
    void println();
    
    bool isLineModeSet();
    void setLineMode(bool value = true);
    
  protected:
    bool _lineMode = true;

    void handleInput();
};

/* ------------------------------------------------- */

  // << operator
  template<class T> inline ESPTelnet &operator <<(ESPTelnet &obj, T arg) { obj.print(arg); return obj; } 

/* ------------------------------------------------- */
#endif
/* ------------------------------------------------- */