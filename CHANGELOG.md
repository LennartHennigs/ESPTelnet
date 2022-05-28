# Changelog

## Unreleased (but available on Github)

- added `print()` and `println()` for different bases and `Printable`as suggested by [francois](https://github.com/lekrom) in [#21](https://github.com/LennartHennigs/ESPTelnet/pull/21)

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
