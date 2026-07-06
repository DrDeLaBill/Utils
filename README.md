# Utils Library

Русская версия: [README.ru.md](README.ru.md)

Cross-platform C/C++ utilities library for embedded and host environments.

The project provides:
- C modules for core runtime utilities, containers, and state machines.
- C++17 helper layer (timers, typelist/meta tools, PID wrapper usage).
- Portable time API with platform-specific branches (Zephyr, FreeRTOS, HAL, Arduino, host).
- Extensive GoogleTest-based regression suite (Debug and Release).

## Table of Contents

1. Overview
2. Project Layout
3. Requirements
4. Build and Test
5. Using the Library in Other CMake Projects
6. Platform Portability Notes
7. Module Guide
8. Archiver Format and API
9. Docker CI Build
10. Troubleshooting
11. References

## 1. Overview

Library target: `utilslib` (static library)

Test target: `utilstest` (GoogleTest executable)

Primary goals:
- Reusable utility components for MCU and desktop simulation.
- Stable behavior in both Debug and Release.
- Predictable API for C and C++ call sites.

## 2. Project Layout

Current logical layout:

```text
inc/utils/
  core/        # C headers: time, logging, strings, utils, version, debug, archiver
  containers/  # C/C++ buffers and sort helpers
  patterns/    # FSM, GSTATE, GPID
  cxx_meta/    # C++17 helpers (timer, typelist, stopwatch, etc.)

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
  test.cpp     # test runner main
```

Top-level CMake scans sources/headers recursively from `inc` and `src`.

## 3. Requirements

- CMake >= 3.18
- C and C++ compilers with C++17 support
- For tests: network access on first configure (FetchContent downloads GoogleTest)

Known environments already used:
- MSVC (Windows)
- GCC (Linux, including Docker image)

## 4. Build and Test

### 4.1 Configure

```powershell
cmake -S . -B build
```

### 4.2 Build Debug and Release

```powershell
cmake --build build --config Debug
cmake --build build --config Release
```

### 4.3 Run tests

```powershell
ctest --test-dir build -C Debug --output-on-failure
ctest --test-dir build -C Release --output-on-failure
```

### 4.4 Typical clean rebuild

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

## 5. Using the Library in Other CMake Projects

You can include this repository as a subdirectory and link with `utilslib`.

Example:

```cmake
add_subdirectory(path/to/Utils)
target_link_libraries(your_target PRIVATE utilslib)
```

Public include directories are exported automatically from `inc/utils/**`.

## 6. Platform Portability Notes

Time API functions:
- `getMillis()`
- `getMillis64bit()`
- `getMicroseconds()`

Implemented platform branches include:
- Zephyr (`__ZEPHYR__`)
- ESP (`ESP_PLATFORM`)
- STM32 HAL (`USE_HAL_DRIVER`)
- Arduino (`ARDUINO`)
- FreeRTOS (`INC_FREERTOS_H` or `FREERTOS`)
- Host GCC (`__GNUC__ && !__arm__`)
- MSVC (`_MSC_VER`)

Bare-metal AVR behavior:
- For `__AVR__` without `ARDUINO`, all three time APIs return `0` by design.
- Rationale: tiny bare-metal targets may require app-specific timer backends.
- If needed, provide your own platform implementation by overriding these weak symbols (where applicable) or by integrating a board timer source.

## 7. Module Guide

### 7.1 Core (C)

- `gtime`: cross-platform timing and simple C timer struct (`gtimer_t`)
- `glog`: print helpers and message-rate filtering
- `gstring`: formatting and numeric string helpers
- `gutils`: generic utility math/memory helpers
- `gversion`: semantic version parse/compare helpers
- `bedug`/`bmacro`: assert and debug macro utilities
- `archiver`: currently stubbed API

### 7.2 Containers

- `circle_buf_gc` (C ring buffer)
- `queue_gc` (C queue wrapper over ring buffer)
- `CircleBuffer` (C++ template ring buffer)
- `GQueue`, `GStack`, `gsort`, `gmedian`

### 7.3 Patterns

- `fsm_gc`: event-driven finite state machine
- `gstate`: queued priority state requests
- `gpid`: PID controller implementation

### 7.4 C++ Meta

- `GTimer`
- `CodeStopwatch`
- Typelist/meta headers (`TypeListBuilder`, `TypeListService`)
- Serialization helpers (`variables.hpp`)

## 8. Archiver Format and API

The archiver is now implemented as a deterministic frame-based `STORE` codec with integrity checks.

Frame header fields:
- `magic` (`0x5241`)
- `version` (`1`)
- `algo` (`ARCHIVER_ALGO_STORE`)
- `src_len`
- `payload_len`
- `crc16` (CRC16-CCITT over payload)

Current API (`archiver.h`):
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

Implemented tests include:
- successful roundtrip `zip -> unzip`
- CRC corruption detection (`ARCHIVER_BAD_CRC`)
- destination-too-small handling (`ARCHIVER_NO_SPACE`)

Planned extensions:
- add compressed algorithms (e.g. RLE/LZ4 block mode)
- keep backward compatibility via `version` + `algo` fields

## 9. Docker CI Build

The repository includes a Dockerfile that:
- configures and builds Debug
- runs tests
- configures and builds Release
- runs tests

Run:

```bash
docker build --no-cache --progress=plain .
```

## 10. Troubleshooting

### CMake configure fails
- Ensure CMake >= 3.18.
- Ensure internet access for GoogleTest download on first configure.

### No tests discovered
- Verify `test/CMakeLists.txt` is included from root CMake.
- Reconfigure project after adding new test files.

### Time values are always zero on AVR
- This is expected for non-Arduino bare-metal AVR branch.
- Provide board-specific timer implementation.

### library.json paths
- `library.json` still references legacy folder names (`utils/...`).
- Current CMake build uses `inc` and `src` layout.
- If PlatformIO packaging is required, update include/srcFilter paths accordingly.

## 11. References

FSM and architecture:
- https://habr.com/ru/articles/540984/

Typelist and meta-programming:
- https://habr.com/ru/articles/220217/
- https://evetro.wordpress.com/2014/05/11/cxx-typelist/
- https://github.com/dutor/loki

Variant and static polymorphism:
- https://habr.com/ru/companies/otus/articles/546158/
- https://www.cppstories.com/2020/09/replacing-into-variant.html/
- https://www.youtube.com/watch?v=gKbORJtnVu8

C/C++ notes:
- https://habr.com/ru/articles/490850/
- https://habr.com/ru/articles/334988/
- https://habr.com/ru/articles/351970/
- https://habr.com/ru/articles/205772/
- https://en.cppreference.com/w/cpp/language/attributes/nodiscard
- https://habr.com/ru/articles/587644/
- https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html
- https://stackoverflow.com/a/73177193