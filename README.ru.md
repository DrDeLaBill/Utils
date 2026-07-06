# Utils Library (RU)

English version: [README.md](README.md)

Кроссплатформенная библиотека C/C++ утилит для embedded и host-сценариев.

Библиотека содержит:
- C-модули для базовых сервисов, контейнеров и автоматов состояний.
- C++17-слой утилит (таймеры, typelist/meta, stopwatch).
- Портируемый API времени с платформенными ветками.
- Набор регрессионных тестов на GoogleTest (Debug/Release).

## Содержание

1. Обзор
2. Структура проекта
3. Требования
4. Сборка и тесты
5. Подключение в другом CMake-проекте
6. Заметки по портируемости
7. Обзор модулей
8. Формат и API Archiver
9. Docker CI сборка
10. Диагностика проблем

## 1. Обзор

Цели:
- Переиспользуемые утилиты для MCU и desktop-симуляции.
- Стабильное поведение в Debug и Release.
- Единый API для C и C++ потребителей.

Основные цели CMake:
- библиотека: `utilslib`
- тесты: `utilstest`

## 2. Структура проекта

```text
inc/utils/
  core/        # C-заголовки: time, log, string, utils, version, debug, archiver
  containers/  # C/C++ буферы/очереди и сортировка
  patterns/    # FSM, GSTATE, GPID
  cxx_meta/    # C++17 meta/utility слой

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

## 3. Требования

- CMake >= 3.18
- компилятор C/C++ с поддержкой C++17
- для первой конфигурации тестов нужен доступ в сеть (FetchContent GoogleTest)

Проверенные окружения:
- Windows + MSVC
- Linux + GCC

## 4. Сборка и тесты

Конфигурация:

```powershell
cmake -S . -B build
```

Сборка:

```powershell
cmake --build build --config Debug
cmake --build build --config Release
```

Запуск тестов:

```powershell
ctest --test-dir build -C Debug --output-on-failure
ctest --test-dir build -C Release --output-on-failure
```

## 5. Подключение в другом CMake-проекте

```cmake
add_subdirectory(path/to/Utils)
target_link_libraries(your_target PRIVATE utilslib)
```

Публичные include-директории экспортируются из `inc/utils/**`.

## 6. Заметки по портируемости

API времени:
- `getMillis()`
- `getMillis64bit()`
- `getMicroseconds()`

Есть ветки для:
- Zephyr
- ESP
- STM32 HAL
- Arduino
- FreeRTOS
- Host GCC
- MSVC

Отдельно для bare-metal AVR (`__AVR__` без `ARDUINO`):
- все функции времени возвращают `0`.
- это ожидаемо, чтобы пользователь подключал board-specific таймеры.

## 7. Обзор модулей

Core:
- `gtime`, `glog`, `gstring`, `gutils`, `gversion`, `bedug`, `bmacro`, `archiver`

Containers:
- `circle_buf_gc`, `queue_gc`, `CircleBuffer`, `GQueue`, `GStack`, `gsort`

Patterns:
- `fsm_gc`, `gstate`, `gpid`

C++ meta:
- `GTimer`, `CodeStopwatch`, typelist/meta заголовки

## 8. Формат и API Archiver

`archiver` реализован как детерминированный frame-based codec в режиме `STORE` (без сжатия) с проверкой целостности `CRC16-CCITT`.

Поля заголовка кадра:
- `magic` (`0x5241`)
- `version` (`1`)
- `algo` (`ARCHIVER_ALGO_STORE`)
- `src_len`
- `payload_len`
- `crc16` (по payload)

API:
- `archiver_status_t zip(const uint8_t* src, uint8_t* arch, uint32_t src_len, uint32_t arch_len)`
- `archiver_status_t unzip(const uint8_t* arch, uint8_t* dst, uint32_t arch_len, uint32_t dst_len)`
- `uint16_t archiver_crc16_ccitt(const uint8_t* data, uint32_t len)`
- `uint32_t archiver_min_frame_size(void)`

Коды статуса:
- `ARCHIVER_OK`
- `ARCHIVER_BAD_ARGS`
- `ARCHIVER_NO_SPACE`
- `ARCHIVER_BAD_FRAME`
- `ARCHIVER_BAD_CRC`
- `ARCHIVER_UNSUPPORTED_ALGO`

Покрытые сценарии тестами:
- roundtrip `zip -> unzip`
- детекция битого CRC
- недостаточный размер destination buffer

## 9. Docker CI сборка

```bash
docker build --no-cache --progress=plain .
```

Dockerfile собирает и прогоняет тесты в Debug и Release.

## 10. Диагностика проблем

- Если CMake не конфигурируется: проверьте версию CMake и доступ в сеть.
- Если тесты не обнаружены: перезапустите configure после добавления новых файлов в `test/`.
- Если на AVR время всегда `0`: это штатное поведение bare-metal ветки, подключите собственный таймер backend.
