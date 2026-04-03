# 🔍 Find Global Opt: Библиотека для поиска глобального минимума функций

[![CMake](https://img.shields.io/badge/CMake-3.14+-brightgreen.svg)](https://cmake.org/)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Tests](https://img.shields.io/badge/🧪-GoogleTest-orange.svg)](https://github.com/google/googletest)
[![License](https://img.shields.io/badge/📄-MIT-yellow.svg)](LICENSE)

**Find Global Opt** — это высокопроизводительная библиотека на C++ для поиска глобального минимума многомерных функций с использованием современных методов глобальной оптимизации. 📈

## ✨ Особенности

- 🎯 **Дифференциальная эволюция (DE)** — популярный метод эволюционной оптимизации
- 🌐 **SHGO (Stochastic Hierarchical Global Optimization)** — иерархический стохастический метод для сложных ландшафтов
- 📊 **Поддержка тестовых функций**: GKLS, Grishagin, Hill, Shekel [Репозиторий функций](https://github.com/OptimLLab/GCGen/tree/master)
- ⚡ **Высокая производительность** благодаря современному C++17
- 🧪 **Полное покрытие тестами** с GoogleTest
- 🕒 **Измерение времени выполнения** функций
- 📈 **Визуализация** процесса оптимизации
- 🔧 **Простая интеграция** в существующие проекты

## 🚀 Быстрый старт

### 📋 Предварительные требования

- **CMake** 3.14 или выше
- **Компилятор C++17** (GCC 7+, Clang 5+, MSVC 2017+)
- **Git** для клонирования зависимостей

### 📥 Установка и сборка

```bash
# 1. Клонируйте репозиторий и загрузите сабмодули
git clone --recursive  https://github.com/Egor1dzeN/global-opt find_global_opt
cd find_global_opt

# 2. Создайте директорию сборки
mkdir build && cd build

# 3. Сконфигурируйте проект
cmake -DCMAKE_BUILD_TYPE=Release ..

# 4. Соберите проект
cmake --build . --config Release --parallel

# 5. Запустите пример
./find_global_opt
```

## 📁 Структура проекта

```
find_global_opt/
├── 📁 include/                           # Заголовочные файлы библиотеки
│   ├── 📁 differential_evolution/        # 🎯 Алгоритм дифференциальной эволюции
│   │   └── differential_evolution.h
│   ├── 📁 shgo/                          # 🌐 SHGO алгоритм
│   │   ├── shgo.h                        # Основной интерфейс SHGO
│   │   ├── delaunay.h                    # Триангуляция Делоне
│   │   ├── edge.h                        # Структура ребра
│   │   ├── simplex.h                     # Симплекс для поиска локального минимума
│   │   ├── hooke_jeeves.h                # Метод Хука-Дживса
│   │   ├── tools.h                       # Вспомогательные инструменты
│   │   └── measuring_time.h              # ⏱️  Измерение времени
│   ├── optimize_result.h                 # 📊 Структура результата оптимизации
│   └── measuring_time.h                  # ⏱️  Измерение времени (глобальный)
├── 📁 src/                               # Исходные файлы
│   ├── 📁 differential_evolution/        # Реализация DE
│   │   └── differential_evolution.cpp
│   ├── 📁 shgo/                          # Реализация SHGO
│   │   └── shgo.cpp
│   └── measuring_time.cpp
├── 📁 example/                           # 📖 Примеры использования
│   └── main.cpp
├── 📁 tests/                             # 🧪 Тесты
│   └── *test.cpp
├── 📁 visualization/                     # 📈 Визуализация алгоритмов
│   ├── animation.py
│   └── functions/
├── 📁 3rd_party/                         # 📦 Зависимости
│   └── GCGen/                            # 📊 Генератор тестовых функций
├── 📄 CMakeLists.txt                     # 🛠️  Сборка проекта
└── 📄 README.md                          # 📋 Эта документация
```

## 💡 Использование

### Пример 1: Дифференциальная эволюция (DE)

```cpp
#include "Hill/HillProblem.hpp" //Header from GCGen repository
#include "differential_evolution/differential_evolution.h"
#include <iostream>

int main() {
    THillProblem tHillProblem;
    size_t input_size = 1;
    constexpr int count_generation = 100;
    auto res = differential_evolution([&](const std::vector<double> &x) -> double {
        return tHillProblem.ComputeFunction(x);
    }, input_size, -1.0, 1.0, count_generation);
    
    std::cout << "Minimum value: " << res.first << std::endl;
    std::cout << "Argument for minimum: (";
    for (size_t i = 0; i < res.second.size(); ++i) {
        std::cout << res.second[i];
        if (i < res.second.size() - 1) std::cout << ", ";
    }
    std::cout << ")" << std::endl;
    return 0;
}
```

### Пример 2: SHGO (Stochastic Hierarchical Global Optimization)

```cpp
#include "Hill/HillProblem.hpp"
#include "shgo/shgo.h"
#include <iostream>

int main() {
    THillProblem tHillProblem;
    size_t input_size = 2;
    
    // Определяем границы поиска
    std::vector<std::pair<double, double>> bounds(input_size, {-1.0, 1.0});
    
    auto res = shgo([&](const std::vector<double> &x) -> double {
        return tHillProblem.ComputeFunction(x);
    }, bounds);
    
    std::cout << "Minimum value: " << res.minimum_value << std::endl;
    std::cout << "Argument for minimum: (";
    for (size_t i = 0; i < res.minimizers.size(); ++i) {
        std::cout << res.minimizers[i];
        if (i < res.minimizers.size() - 1) std::cout << ", ";
    }
    std::cout << ")" << std::endl;
    return 0;
}
```

## 🔄 Сравнение методов оптимизации

| Характеристика | Дифференциальная эволюция | SHGO |
|---|---|---|
| **Тип** | Эволюционный алгоритм | Иерархический стохастический |
| **Скорость** | Быстрая | Более медленная, но точнее |
| **Надежность** | Хорошая | Очень высокая |
| **Сложность функции** | Лучше для простых | Отличная для сложных ландшафтов |
| **Рекомендуется для** | Реал-тайм задачи | Критичная точность |

## 📊 Поддерживаемые тестовые функции

Библиотека включает генератор тестовых функций **GCGen** из [Репозитория](https://github.com/OptimLLab/GCGen):

- **GKLS** функции (многомерные) — сложные многомодальные функции
- **Grishagin** функции — хорошо изученные тестовые функции
- **Hill** функции — функции с плато
- **Shekel** функции — функции с острыми пиками

## 📊 Визуализация результатов

Библиотека включает скрипты Python для визуализации процесса оптимизации:

```bash
cd visualization
python animation.py  # Генерирует анимацию поиска минимума
```

Поддерживаемые функции для визуализации:
- Функция Розенброка (Rosenbrock)
- Другие 2D/3D функции

## 🧪 Запуск тестов

```bash
cd build
./find_global_opt_test  # Прямой запуск тестового исполняемого файла
```

Тесты покрывают все поддерживаемые тестовые функции:
- GKLS test
- Grishagin test
- Hill test  
- Shekel test

## 📦 Интеграция в ваш проект

### Через CMake (рекомендуется)

```cmake
# В вашем CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
    find_global_opt
    GIT_REPOSITORY https://github.com/Egor1dzeN/global-opt
    GIT_TAG main  # или конкретная версия
)
FetchContent_MakeAvailable(find_global_opt)

# Связываем с вашим проектом
target_link_libraries(<your name project> PRIVATE find_global_opt)
target_include_directories(<your name project> PRIVATE ${find_global_opt_SOURCE_DIR}/include)
```

## ⚙️ Параметры оптимизации

| Параметр | Описание | По умолчанию           |
|----------|----------|------------------------|
| `maxIterations` | 🔄 Максимальное количество итераций | Задается пользователем |
| `populationSize` | 👥 Размер популяции | 40                     |
| `dimension` | 📐 Размерность пространства поиска | Задается пользователем |
| `crossoverRate` | 🔀 Вероятность кроссовера | 0.9                    |
| `differentialWeight` | ⚖️ Дифференциальный вес | 0.8                    |
| `bounds` | 📏 Границы поиска | Задается пользователем            |

## 📊 Результаты оптимизации

### Результат Дифференциальной эволюции

```cpp
std::pair<double, std::vector<double>> 
// first: минимальное значение функции
// second: аргумент, при котором достигается минимум
```

### Результат SHGO

```cpp
struct OptimizeResult {
    double minimum_value;              // Найденное минимальное значение
    std::vector<double> minimizers;    // Аргумент минимума
    int num_function_calls;            // Количество вызовов функции
    bool success;                      // Успешно ли завершена оптимизация
    std::string message;               // Сообщение о статусе
}
```

## 🛠️ Сборка для разработки

```bash
# Сборка с отладочной информацией
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --parallel

# Сборка с санитайзерами
cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZERS=ON ..
```





## 📄 Лицензия

Этот проект распространяется под лицензией MIT. Подробнее см. в файле [LICENSE](LICENSE).


