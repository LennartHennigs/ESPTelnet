/* ------------------------------------------------- */
/*
 * heavily inspired by 
 *  https://github.com/jerabaul29/ArduinoDebugMacros/
 *  https://forum.arduino.cc/index.php?topic=46900.0
 *  https://forum.arduino.cc/index.php?topic=215334.0
*/
/* ------------------------------------------------- */

#pragma once

#ifndef DebugMacros_h
#define DebugMacros_h

/* ------------------------------------------------- */

#ifndef DEBUG_SERIAL
  #define DEBUG_SERIAL            Serial
#endif
#ifndef DEBUG_TELNET
  #define DEBUG_TELNET            telnet
#endif
#ifndef DEBUG_PREFIX
  #define DEBUG_PREFIX            "[DEBUG]\t"
#endif

/* ------------------------------------------------- */

#ifndef DEBUG_ON
  #define DEBUG_INFO
  #define DEBUG_MSG(x)
  #define DEBUG_VAR(...)
  #define DEBUG_WHERE
#else 
  #if DEBUG_USE_SERIAL
    #define DEBUG_SER_INFO      DEBUG_SERIAL.print(DEBUG_PREFIX); DEBUG_SERIAL.println("Compiled " __DATE__ ", " __TIME__)
    #define DEBUG_SER_MSG(x)    DEBUG_SERIAL.print(DEBUG_PREFIX); DEBUG_SERIAL.println(x)
    #define DEBUG_SER_VAR(...)  DEBUG_SERIAL.print(DEBUG_PREFIX); DEBUG_SERIAL.print(F(#__VA_ARGS__ " = ")); DEBUG_SERIAL.println(__VA_ARGS__)
    #define DEBUG_SER_WHERE     DEBUG_SERIAL.print(DEBUG_PREFIX); DEBUG_SERIAL.print(F(__FILE__ " - ")); DEBUG_SERIAL.print(__PRETTY_FUNCTION__); DEBUG_SERIAL.print(": "); DEBUG_SERIAL.println(__LINE__)
  #else
    #define DEBUG_SER_INFO
    #define DEBUG_SER_MSG(x)
    #define DEBUG_SER_VAR(...)
    #define DEBUG_SER_WHERE
  #endif

  #if DEBUG_USE_TELNET
    #define DEBUG_TEL_INFO      DEBUG_TELNET.print(DEBUG_PREFIX); DEBUG_TELNET.println("Compiled " __DATE__ ", " __TIME__)
    #define DEBUG_TEL_MSG(x)    DEBUG_TELNET.print(DEBUG_PREFIX); DEBUG_TELNET.println(x)
    #define DEBUG_TEL_VAR(...)  DEBUG_TELNET.print(DEBUG_PREFIX); DEBUG_TELNET.print(F(#__VA_ARGS__  " = ")); telnet.println(String(__VA_ARGS__))
    #define DEBUG_TEL_WHERE     DEBUG_TELNET.print(DEBUG_PREFIX); DEBUG_TELNET.print(F(__FILE__ " - ")); DEBUG_TELNET.print(__PRETTY_FUNCTION__); DEBUG_TELNET.print(": "); DEBUG_TELNET.println(String(__LINE__))
  #else
    #define DEBUG_TEL_INFO
    #define DEBUG_TEL_MSG(x)
    #define DEBUG_TEL_VAR(...)
    #define DEBUG_TEL_WHERE
  #endif

  #define DEBUG_INFO            DEBUG_SER_INFO; DEBUG_TEL_INFO
  #define DEBUG_MSG(x)          DEBUG_SER_MSG(x); DEBUG_TEL_MSG(x)
  #define DEBUG_VAR(...)        DEBUG_SER_VAR(__VA_ARGS__); DEBUG_TEL_VAR(__VA_ARGS__)
  #define DEBUG_WHERE           DEBUG_SER_WHERE; DEBUG_TEL_WHERE
#endif

/* ------------------------------------------------- */
#endif
/* ------------------------------------------------- */
