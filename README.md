# 🔍 Find Global Opt: Библиотека для поиска глобального минимума функций

[![CMake](https://img.shields.io/badge/CMake-3.14+-brightgreen.svg)](https://cmake.org/)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Tests](https://img.shields.io/badge/🧪-GoogleTest-orange.svg)](https://github.com/google/googletest)
[![License](https://img.shields.io/badge/📄-MIT-yellow.svg)](LICENSE)

**Find Global Opt** — это высокопроизводительная библиотека на C++ для поиска глобального минимума многомерных функций с
использованием современных методов глобальной оптимизации. 📈

## ✨ Особенности

- 🎯 **Дифференциальная эволюция (DE)** — популярный метод эволюционной оптимизации
- 🌐 **SHGO (Stochastic Hierarchical Global Optimization)** — иерархический стохастический метод для сложных ландшафтов
- 📊 **Поддержка тестовых функций**: GKLS, Grishagin, Hill,
  Shekel [Репозиторий функций](https://github.com/OptimLLab/GCGen/tree/master)
- ⚡ **Высокая производительность** благодаря современному C++17
- 🧪 **Полное покрытие тестами** с GoogleTest
- 🕒 **Измерение времени выполнения** функций
- 📈 **Визуализация** процесса оптимизации
- 🔧 **Простая интеграция** в существующие проекты

## 🚀 Быстрый старт

### 📋 Предварительные требования

- **CMake** 3.31 или выше
- **Компилятор C++17** (GCC 11+, Clang 14+, MSVC 2022+)
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
│   └── measuring_time.h                  # ⏱️  Измерение времени (глобальный)
├── 📁 src/                               # Исходные файлы
│   ├── 📁 differential_evolution/        # Реализация DE
│   │   └── differential_evolution.cpp
│   ├── 📁 shgo/                          # Реализация SHGO
│   │   └── shgo.cpp
│   ├── measuring_time.cpp
│   └── python_bindings.cpp               # 🐍 Привязки Python (pybind11)
├── 📁 example/                           # 📖 Примеры использования
│   └── main.cpp
├── 📁 tests/                             # 🧪 Тесты
│   ├── 📁 differential_evolution/        # Тесты DE
│   │   └── *test.cpp
│   └── 📁 shgo/                          # Тесты SHGO
│       └── *test.cpp
├── 📁 tools/                             # 🔧 Вспомогательные инструменты
├── 📁 visualization/                     # 📈 Визуализация алгоритмов
│   ├── animation.py
│   ├── plot2D.py
│   ├── plot3D.py
│   ├── requirements.txt
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
    }, {std::make_pair(-1., 1.)});
    std::cout << "Minimum value: " << res.fun << std::endl;
    std::cout << "Argument for minimum: (";
    for (size_t i = 0; i < res.x.size(); ++i) {
        std::cout << res.x[i];
        if (i < res.x.size() - 1) std::cout << ", ";
    }
    std::cout << ")" << std::endl;
    std::cout << "Global minimum: " << tHillProblem.GetOptimumValue() << "\n";
    std::cout << "Call count: " << res.nfev << "\n";
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
    auto res = shgo([&](const std::vector<double> &x) -> double {
        return tHillProblem.ComputeFunction(x);
    }, {std::make_pair(-1., 1.)});
    std::cout << "Minimum value: " << res.fun << std::endl;
    std::cout << "Argument for minimum: (";
    for (size_t i = 0; i < res.x.size(); ++i) {
        std::cout << res.x[i];
        if (i < res.x.size() - 1) std::cout << ", ";
    }
    std::cout << ")" << std::endl;
    std::cout<<"Global minimum: "<<tHillProblem.GetOptimumValue()<<"\n";
    std::cout<<"Call count: "<<res.nfev<<"\n";
    return 0;
}
```

## 🔄 Сравнение методов оптимизации

| Характеристика        | Дифференциальная эволюция | SHGO                            |
|-----------------------|---------------------------|---------------------------------|
| **Тип**               | Эволюционный алгоритм     | Иерархический стохастический    |
| **Скорость**          | Быстрая                   | Более медленная, но точнее      |
| **Надежность**        | Хорошая                   | Очень высокая                   |
| **Сложность функции** | Лучше для простых         | Отличная для сложных ландшафтов |
| **Рекомендуется для** | Реал-тайм задачи          | Критичная точность              |

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

## 📊 Результаты оптимизации

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


