ESPTelnet
=========

ESP8266/ESP32 library that allows you to setup a telnet server.

* Author: Lennart Hennigs (https://www.lennarthennigs.de)
* Copyright (C) 2018-2022 Lennart Hennigs.
* Released under the MIT license.

Description
-----------
Use this library to set up a the telnet server to communicate status messages or debug / error log output. This is especially useful when you don't have a serial connection to the ESP.

It has been tested with ESP8266 and ESP32 devices.

To see the latest changes to the library please take a look at the [Changelog](https://github.com/LennartHennigs/ESPTelnet/blob/master/CHANGELOG.md).

If you find this library helpful please consider giving it a ⭐️ at [GitHub](https://github.com/LennartHennigs/ESPTelnet) and/or [buy me a ☕️](https://ko-fi.com/lennart0815). Thanks!



How To Use
----------

__Definition__
- Use the `begin()` and `stop()` to start or stop the telnet server. 
- It needs an active WiFi connection, or the ESP needs to be in softAP mode
- You can set a custom port (other than 23) via the `begin()` function

- The telnet server only allows you to connect a single client to it. 
- You can use `getIP()` to get the connected client's IP address.
- You manually disconnect the client via `disconnectClient()`

__Callback Handlers__
- The library uses callback handlers to notify you of different telnet events
  - ```void onConnect(CallbackFunction f);```
  - ```void onConnectionAttempt(CallbackFunction f);```
  - ```void onReconnect(CallbackFunction f);```
  - ```void onDisconnect(CallbackFunction f);```
  - ```void onInputReceived(CallbackFunction f);```

- All callback functions need a String parameter. There the IP from the connected client is sen, or the input text in case of `onInputReceived()`.

__Output and Input__
- Via `print()` and `println()` you can output text on the telnet server.
- To receive and parse input from the telnet client you can add a handler via `onInputReceived()`.
- By default, the library waits for a newline character from the client, and sends data to the callback handler one line at a time. This behaviour can be deactivated by calling `setlineMode(false)`.

__Using it to Debug Code__    
- A common use case is to use it for debuging remote devices, where you might not have access to Serial. Thus I added a few macros to make debugging easier:

- To enable debugging you need to set `DEBUG_ON`
  ```#define DEBUG_ON 1```
- You can then define the output channels (serial and/or telnet)
  ```
  #define DEBUG_USE_SERIAL 1
  #define DEBUG_USE_TELNET 1
  ```

- After this you can use the following macros
  - `DEBUG_INFO` to see compilation the date & time of your sketch
  - `DEBUG_WHERE` to print where this message is located (file & line number)
  - `DEBUG_MSG(x)` to print a message
  - `DEBUG_VAR(...)` to print a variable value

- See the [DebugMacroExample](https://github.com/LennartHennigs/ESPTelnet/blob/master/examples/DebugMacroExample/DebugMacroExample.ino) for more details.

__The Loop__    
- For the class to work, you need to call the `loop()` member function in your sketch's `loop()` function. 


Examples
--------
- [TelnetServerExample](https://github.com/LennartHennigs/ESPTelnet/blob/master/examples/TelnetServerExample/TelnetServerExample.ino) – basic example
- [TelnetServerExampleWithWiFiManager](https://github.com/LennartHennigs/ESPTelnet/blob/master/examples/TelnetServerWithWiFiManager/TelnetServerWithWiFiManager.ino) – basic example using [WiFiManager](https://github.com/tzapu/WiFiManager)
- [DebugMacroExample](https://github.com/LennartHennigs/ESPTelnet/blob/master/examples/DebugMacroExample/TelnetServerExample.ino) – to see the debug macros in action
 
Notes
-----

- To see the latest changes to the library please take a look at the [Changelog](https://github.com/LennartHennigs/ESPTelnet/blob/master/CHANGELOG.md).
 
- And if you find this library helpful, please consider giving it a star at [GitHub](https://github.com/LennartHennigs/ESPTelnet). Thanks!


Class Definition
----------------
These are the constructors and the member functions the library provides:

```
    ESPTelnet();

    bool begin();
    void loop();
    void stop();

    void print(String str);
    void print(char c);
    void println(String str);
    void println(char c);
    void println();

    String getIP() const;
    String getLastAttemptIP() const;
    
    void onConnect(CallbackFunction f);
    void onConnectionAttempt(CallbackFunction f);
    void onReconnect(CallbackFunction f);
    void onDisconnect(CallbackFunction f);
    void onInputReceived(CallbackFunction f);

    void disconnectClient();
```



Installation
------------
Open the Arduino IDE choose "Sketch > Include Library" and search for "ESPTelnet". 
Or download the ZIP archive (https://github.com/lennarthennigs/ESPTelnet/zipball/master), and choose "Sketch > Include Library > Add .ZIP Library..." and select the downloaded file.


License
-------

MIT License

Copyright (c) 2018-2021 Lennart Hennigs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
