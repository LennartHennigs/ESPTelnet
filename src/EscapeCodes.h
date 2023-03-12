/////////////////////////////////////////////////////////////////
/*
    Library that returns ANSI escape sequences as Strings

    inspiried by Bruce Robertson's ANSITerm
    - https://forum.arduino.cc/t/ansiterm-a-ansi-terminal-library/37536/1
    - https://github.com/rklancer/ansiterm

    - https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

*/
/////////////////////////////////////////////////////////////////

#include <Arduino.h>

/////////////////////////////////////////////////////////////////

#pragma once

#ifndef EscapeCodes_h
#define EscapeCodes_h

/////////////////////////////////////////////////////////////////

#define ANSI_BLACK 0
#define ANSI_RED 1
#define ANSI_GREEN 2
#define ANSI_YELLOW 3
#define ANSI_BLUE 4
#define ANSI_MAGENTA 5
#define ANSI_CYAN 6
#define ANSI_WHITE 7
#define ANSI_GREY 60
#define ANSI_BRIGHT_RED 61
#define ANSI_BRIGHT_GREEN 62
#define ANSI_BRIGHT_YELLOW 63
#define ANSI_BRIGHT_BLUE 64
#define ANSI_BRIGHT_MAGENTA 65
#define ANSI_BRIGHT_CYAN 66
#define ANSI_BRIGHT_WHITE 67

/////////////////////////////////////////////////////////////////

class EscapeCodes {
 public:
  static String home();
  static String cls();
  static String clearLine();
  static String clearEoLine();

  static String cursorXY(int x, int y);
  static String cursorUp(int x);
  static String cursorDown(int x);
  static String cursorLeft(int x);
  static String cursorRight(int x);

  static String setBG(int color);
  static String setFG(int color);

  static String bold(String str);
  static String italic(String str);
  static String underline(String str);
  static String blink(String str);
  static String inverse(String str);

  static String showCursor(bool blink);
  static String reset();

 protected:
  static String prefix();
  static String prefixAndNumberAndValue(int x, char v);
  static String setAttribute(int a);
};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
