# Utils Library

Английская версия: [README.md](README.md)

Кроссплатформенная библиотека C/C++ утилит для embedded и host-проектов. Текущий код организован как отдельная CMake-библиотека с публичными заголовками в `inc/` и реализацией в `src/`.

## 1. Подключение библиотеки

`utilslib` - это статическая CMake-цель, которую экспортирует этот проект. Добавьте библиотеку как поддиректорию и свяжите её с нужной целью:

```cmake
add_subdirectory(path/to/Utils)
target_link_libraries(your_target PRIVATE utilslib)
```

После линковки публичные заголовки подключаются автоматически, поэтому можно включать файлы из `core/`, `containers/`, `patterns/` и `cxx_meta/` напрямую, например:

```cpp
#include "gtime.h"
#include "CircleBuffer.hpp"
```

Если нужен только набор заголовков в кастомной системе сборки, добавьте `src/Utils/inc` в include path и соберите соответствующие файлы из `src/Utils/src`.

## 2. Обзор

Библиотека содержит:

- C-модули для runtime-хелперов, строк, версий, времени, отладки, архивации, контейнеров и автоматов состояний.
- C++17-хелперы для таймеров, typelist/meta-утилит и небольших вспомогательных обёрток.
- Портируемый API времени с ветками для Zephyr, FreeRTOS, HAL, Arduino, host-сборок и MSVC.
- Набор регрессионных тестов на GoogleTest.

Основные CMake-цели:

- `utilslib` - статическая библиотека
- `utilstest` - исполняемый файл тестов

## 3. Структура проекта

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

Имена модулей соответствуют файлам заголовков и исходников в этих папках.

## 4. Требования

- CMake 3.18 или новее
- компилятор C и C++ с поддержкой C++17
- доступ в сеть на первом configure для загрузки GoogleTest через FetchContent

Проверенные окружения:

- Windows с MSVC
- Linux с GCC

## 5. Сборка и тесты

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

Чистая пересборка:

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

## 6. Обзор модулей

### Core

- `gtime` - кроссплатформенные функции времени и `gtimer_t`
- `gstring` - форматирование строк и числовые хелперы
- `gutils` - общие utility-функции
- `gversion` - парсинг и сравнение версий
- `bedug` - отладочные хелперы и runtime-проверки
- `bmacro` - вспомогательные макросы для отладки и assert
- `archiver` - frame-based store codec с CRC-проверкой

### Containers

- `circle_buf_gc` - C ring buffer
- `queue_gc` - C-очередь поверх ring buffer
- `CircleBuffer` - C++ шаблон кольцевого буфера
- `GQueue` и `GStack` - контейнерные обёртки
- `gsort` - функции сортировки

### Patterns

- `fsm_gc` - автомат состояний, основанный на событиях
- `gstate` - управление очередью состояний
- `gpid` - PID-регулятор

### C++ Meta

- `GTimer`
- `CodeStopwatch`
- `TypeListBuilder` и `TypeListService`
- `glambda`
- `variables.hpp`

## 7. Портируемый API времени

Библиотека предоставляет функции времени:

- `getMillis()`
- `getMillis64bit()`
- `getMicroseconds()`

Поддерживаемые ветки сейчас включают:

- Zephyr
- ESP-IDF
- STM32 HAL
- Arduino
- FreeRTOS
- host-сборки GCC
- MSVC

Для bare-metal AVR без Arduino функции времени возвращают `0` по задумке, потому что board-specific backend должен предоставить сам проект.

## 8. API Archiver

`archiver` реализован как детерминированный `STORE` frame codec с проверкой целостности CRC16-CCITT.

Публичный API из `archiver.h`:

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

## 9. Диагностика проблем

- Если configure не проходит, проверьте версию CMake и доступ в сеть на первом запуске для загрузки GoogleTest.
- Если тесты не находятся, повторите configure после добавления файлов в `test/`.
- Если на AVR время всегда `0`, это штатное поведение bare-metal ветки без Arduino.
- Если используется `library.json`, нужно обновить устаревшие пути `utils/...` под текущую структуру `inc/` и `src/`.
