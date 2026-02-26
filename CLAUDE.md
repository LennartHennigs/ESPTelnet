# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESPTelnet is an Arduino library (v2.2.3) for ESP8266/ESP32 that sets up a single-client telnet server. It's primarily used to send debug/status output to a remote terminal when no Serial connection is available.

## Build Commands (PlatformIO)

```bash
# Build for default environment (d1_mini_lite)
pio run

# Build for a specific board
pio run -e <environment_name>
# e.g.: pio run -e esp32dev
#       pio run -e m5stack-core-esp32
#       pio run -e arduino_nano_esp32

# List all available environments: d1_mini_lite, d1_mini, nodemcuv2, esp12e,
# esp32dev, m5stack-core-esp32, m5stack-fire, m5stick-c, arduino_nano_esp32,
# esp32-s2-saola-1, esp32-c3-devkitm-1
```

There is no automated test suite — validation is done by building against real hardware environments.

## Architecture

The library has a three-class hierarchy in `src/`:

```
ESPTelnetBase (ESPTelnetBase.h / .cpp)
  ├── ESPTelnet (ESPTelnet.h / .cpp)         — print/println/printf + line-mode input
  └── ESPTelnetStream (ESPTelnetStream.h / .cpp) — Arduino Stream interface
```

**`ESPTelnetBase`** owns all connection management:
- `begin()` / `stop()` / `loop()` — lifecycle
- `loop()` calls three internal methods: `processClientConnection()`, `performKeepAliveCheck()`, `handleClientInput()`
- Only one client allowed at a time; reconnection from the same IP is handled via `handleReconnection()`
- Failed writes are counted; after `MAX_ERRORS_ON_WRITE` (3) failures, `disconnectClient()` is called automatically
- Keep-alive check interval defaults to `KEEP_ALIVE_INTERVAL_MS` (1000ms), configurable via `setKeepAliveInterval()`
- `TCPClient` / `TCPServer` are type aliases for `WiFiClient` / `WiFiServer`
- Platform differences (ESP8266 vs ESP32) are handled with `#ifdef ARDUINO_ARCH_ESP8266 / ESP32` guards

**`ESPTelnet`** adds:
- Templated `print()` / `println()` for any type
- `printf()` with dynamic buffer allocation for strings > 64 bytes
- Line-mode input (default on): buffers chars until newline, then fires `onInputReceived` callback
- Character-mode input (`setLineMode(false)`): fires callback per character
- `<<` stream operator

**`ESPTelnetStream`** adds:
- Implements Arduino `Stream` interface (`available()`, `read()`, `peek()`, `write()`, `flush()`)
- Input handling via `handleInput()` queues received bytes as Stream data

**Additional files:**
- `src/EscapeCodes.h / .cpp` — static-method class returning ANSI escape sequence Strings for terminal control (colors, cursor movement, clear screen, etc.)
- `src/DebugMacros.h` — preprocessor macros (`DEBUG_MSG`, `DEBUG_VAR`, `DEBUG_WHERE`, `DEBUG_INFO`) that route output to Serial, the `telnet` object, or both. Enabled by `#define DEBUG_ON 1`. The debug macro for telnet assumes the global variable is named `telnet`.

## Key Design Constraints

- The server only accepts **one concurrent client**.
- `loop()` must be called in the Arduino sketch's `loop()` function every iteration.
- `begin()` checks for WiFi/SoftAP connectivity by default; pass `begin(23, false)` to skip (e.g. for Ethernet adapters).
- All callbacks receive a `String` parameter: the client IP for connection events, the received text for `onInputReceived`.
- The `input` buffer (in `ESPTelnetBase`) is shared state between base and derived classes.
