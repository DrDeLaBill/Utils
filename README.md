# Utils Library

Russian version: [README.ru.md](README.ru.md)

Cross-platform C/C++ utilities library for embedded and host projects. The current codebase is organized as a standalone CMake library with public headers under `inc/` and implementation files under `src/`.

## 1. Connecting the Library

`utilslib` is the static CMake target exported by this project. Add the library as a subdirectory and link it to your target:

```cmake
add_subdirectory(path/to/Utils)
target_link_libraries(your_target PRIVATE utilslib)
```

Public headers are exported automatically, so after linking you can include files from `core/`, `containers/`, `patterns/`, and `cxx_meta/` directly, for example:

```cpp
#include "gtime.h"
#include "CircleBuffer.hpp"
```

If you only need the headers in a custom build system, add `src/Utils/inc` to your include path and compile the matching sources from `src/Utils/src`.

## 2. Overview

The library provides:

- C modules for runtime helpers, strings, versioning, time, debugging, archiving, containers, and state machines.
- C++17 helpers for timers, typelist/meta utilities, and small RAII-style helpers.
- A portable time API with platform branches for Zephyr, FreeRTOS, HAL, Arduino, host builds, and MSVC.
- A GoogleTest-based regression suite for the library code.

Main CMake targets:

- `utilslib` - static library
- `utilstest` - test executable

## 3. Project Layout

```text
inc/
  core/
  containers/
  patterns/
  cxx_meta/

src/
  core/
  containers/
  patterns/
  cxx_meta/

test/
  core/
  containers/
  patterns/
  cxx_meta/
  test.cpp
```

Actual module names follow the header and source files in those folders.

## 4. Requirements

- CMake 3.18 or newer
- C and C++ compilers with C++17 support
- Internet access on the first configure step for GoogleTest download via FetchContent

Known working environments:

- Windows with MSVC
- Linux with GCC

## 5. Build and Test

Configure:

```powershell
cmake -S . -B build
```

Build:

```powershell
cmake --build build --config Debug
cmake --build build --config Release
```

Run tests:

```powershell
ctest --test-dir build -C Debug --output-on-failure
ctest --test-dir build -C Release --output-on-failure
```

Clean rebuild:

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

## 6. Module Guide

### Core

- `gtime` - cross-platform timing helpers and `gtimer_t`
- `gstring` - string formatting and numeric helpers
- `gutils` - generic utility helpers
- `gversion` - version parse and compare helpers
- `bedug` - debug helpers and runtime checks
- `bmacro` - supporting macros for debug and assertions
- `archiver` - frame-based store codec with CRC checks

### Containers

- `circle_buf_gc` - C ring buffer
- `queue_gc` - C queue wrapper over the ring buffer
- `CircleBuffer` - C++ ring buffer template
- `GQueue` and `GStack` - container wrappers
- `gsort` - sorting helpers

### Patterns

- `fsm_gc` - event-driven finite state machine
- `gstate` - queued state management
- `gpid` - PID controller implementation

### C++ Meta

- `GTimer`
- `CodeStopwatch`
- `TypeListBuilder` and `TypeListService`
- `glambda`
- `variables.hpp`

## 7. Portable Time API

The library exposes platform-aware time functions:

- `getMillis()`
- `getMillis64bit()`
- `getMicroseconds()`

Supported branches currently include:

- Zephyr
- ESP-IDF
- STM32 HAL
- Arduino
- FreeRTOS
- host GCC builds
- MSVC

For bare-metal AVR without Arduino, the time functions return `0` by design because the board-specific timer backend is expected to be provided by the application.

## 8. Archiver API

The archiver is implemented as a deterministic `STORE` frame codec with CRC16-CCITT integrity checks.

Public API from `archiver.h`:

- `archiver_status_t zip(const uint8_t* src, uint8_t* arch, uint32_t src_len, uint32_t arch_len)`
- `archiver_status_t unzip(const uint8_t* arch, uint8_t* dst, uint32_t arch_len, uint32_t dst_len)`
- `uint16_t archiver_crc16_ccitt(const uint8_t* data, uint32_t len)`
- `uint32_t archiver_min_frame_size(void)`

Status codes:

- `ARCHIVER_OK`
- `ARCHIVER_BAD_ARGS`
- `ARCHIVER_NO_SPACE`
- `ARCHIVER_BAD_FRAME`
- `ARCHIVER_BAD_CRC`
- `ARCHIVER_UNSUPPORTED_ALGO`

## 9. Troubleshooting

- If CMake configure fails, check the CMake version and first-configure network access for GoogleTest.
- If tests are not discovered, re-run configure after adding files under `test/`.
- If AVR time values are always zero, that is expected for the bare-metal branch without Arduino.
- If you use `library.json`, update the legacy `utils/...` paths to match the current `inc/` and `src/` layout.