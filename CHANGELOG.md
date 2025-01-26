# Changelog

## Unreleased (but available on Github)

**Note:** Unreleased changes are checked in but not part of an official release (available through the Arduino IDE or PlatfomIO) yet. This allows you to test WiP features and give feedback to them.

## [2.2.3] – 2025-01-26

- removed `yield()` from [ESPTelnetBase.cpp](https://github.com/LennartHennigs/ESPTelnet/blob/main/src/ESPTelnetBase.cpp) `loop()` as it creates problems with context switching, see #73
- fixed errors in TelnetStreamExample.ino
 

## [2.2.2] – 2024-10-03

- removed `ESP8266WiFi.h` include in `ESPTelnetBase.h`
- Added option to change the newLine character via `getNewlineCharacter()` and `setNewlineCharacter()` as suggest by [Roy](https://github.com/FunDeckHermit) in [#69](https://github.com/LennartHennigs/ESPTelnet/pull/69)

## [2.2.1] – 2024-02-17

- added `getClient()` function to the base class
- changed order of macros in `DebugMacros.h` to avoid an error

## [2.2] – 2023-12-28

- merged pull request [#56](https://github.com/LennartHennigs/ESPTelnet/pull/56)  that fixes issues #54 and #52 submitted by [Laxilef](https://github.com/Laxilef)
  - added buffered `write()` function to `ESPTelnetBase`as suggested by [Laxilef](https://github.com/Laxilef)
  - Losing client connection will no longer freeze the server
- fixed bug in `connectToWiFi()` helper function

## [2.1.2] - 2022-06-26

- refactored `ESPTelnetBase`, simplified the main `loop()`
- updated `ESPTelnet`, simplified `printf()`
- updated `ESPTelnet`, `print()` and `println()` now uses templates
- updated `ESPTelnetStream` example

## [2.1.1] - 2022-03-12

- made `ESPStream`'s inheritance of `Stream` public as requested in [#45](https://github.com/LennartHennigs/ESPTelnet/issues/45)

## [2.1.0] - 2022-03-12

- simplified and removed parameter from `isClientConnected(TCPClient &client)` to `isConnected()` (BREAKING CHANGE)
- the `loop()`now checks periodically (every second) whether the client "sliently" disconnected, interval for this can be custonmized
- fixed bug in the main `loop()` that stopped processing text input
- removed compiler warnings
- added ANSI escape code library
- added print functions as suggested in [#37](https://github.com/LennartHennigs/ESPTelnet/pull/37) by Jan Heuer
- added printf() altazorr as suggested in [#35](https://github.com/LennartHennigs/ESPTelnet/pull/35) by Bogdan Symchych
- refactored and beautified code

## [2.0.0] - 2022-11-21

- added abstract base class and refactored code
- implemented `ESPTelnet` and a separate stream class `ESPTelnetStream`
- added [ESPTelnetStreamExample](https://github.com/LennartHennigs/ESPTelnet/blob/master/examples/TelnetStreamExample/TelnetStreamExample.ino)
- updated `README.md` and added Stream descriptions
- fixed bug [#30](https://github.com/LennartHennigs/ESPTelnet/issues/30)

## [1.4.0] - 2022-11-03

- added `Autoconnect` example as suggested in PR [#13](https://github.com/LennartHennigs/ESPTelnet/pull/13)
- updated `loop()` to remove compiler warning as mentioned in [#26](https://github.com/LennartHennigs/ESPTelnet/issues/26)
- added `bool checkConnection` parameter to `begin()` to allow to bypass WiFi detection, as requested in [#27](https://github.com/LennartHennigs/ESPTelnet/issues/27)
- added `print()` and `println()` for different bases and `Printable` as suggested in [#21](https://github.com/LennartHennigs/ESPTelnet/pull/21)

## [1.3.1] - 2022-03-15

- Fixed bug with `_isSet()`method as reported in [#19](https://github.com/LennartHennigs/ESPTelnet/issues/19)

## [1.3.0] - 2022-03-03

- Added a line mode toggle as suggested in [#17](https://github.com/LennartHennigs/ESPTelnet/pull/17)
- Changed `String` arguments to `const String &` and `WiFiClient` argument to `WiFiClient &` to avoid copies as requested in pull request [#16](https://github.com/LennartHennigs/ESPTelnet/pull/16)
- You can now define a custom port via the `begin(uint16_t port)` function as requested in [#18](https://github.com/LennartHennigs/ESPTelnet/issues/18)
- Added a `clientDisconnect()` function as requested in [#18](https://github.com/LennartHennigs/ESPTelnet/issues/18)

## [1.2.2] - 2021-12-06

- Added code to manually flush the client upon (re-)connection. This fixes [#9](https://github.com/LennartHennigs/ESPTelnet/issues/9), reported by [Alex](https://github.com/alex-fu27)
- Added private helper method `_isIPSet()`, as ESP32's IPAddress class has no `isSet()` method - this fixes [#12](https://github.com/LennartHennigs/ESPTelnet/issues/12)

## [1.2.1] - 2021-09-26

- Fixed error that `onTelnetDisconnect()` was not called, as suggested by [zenbooster](https://github.com/zenbooster) in [#11](https://github.com/LennartHennigs/ESPTelnet/issues/11)

## [1.2.0] - 2021-09-12

- Added debug macro library file and example
- now able to work in AP mode, not only when connected to a WiFi
- used `println()` for `println()`, as suggested by [JMWilton](https://github.com/JMWilton) in [#10](https://github.com/LennartHennigs/ESPTelnet/issues/10)

## [1.1.1] - 2021-04-09

- added example using WiFiManager - for ESP8266 only
- changed pre-processor directive, as suggested by [Aaron](https://github.com/AFontaine79) in [#4](https://github.com/LennartHennigs/ESPTelnet/issues/4)

## [1.1.0] - 2021-02-03

- cleaned up .h file
- added ip as return parameter to callback functions
- added onInputReceived() function to listen for telnet input
- added ESP32 support

## [1.0.0] - 2020-03-28

- initial release

## Note

- The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).
