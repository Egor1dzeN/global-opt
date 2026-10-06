# 🔍 Find Global Opt: Library for Finding Global Minimum of Functions

[![CMake](https://img.shields.io/badge/CMake-3.31+-brightgreen.svg)](https://cmake.org/)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Tests](https://img.shields.io/badge/🧪-GoogleTest-orange.svg)](https://github.com/google/googletest)
[![License](https://img.shields.io/badge/📄-MIT-yellow.svg)](LICENSE)

**Find Global Opt** — a high-performance C++ library for finding the global minimum of multidimensional functions using modern global optimization methods. 📈

## ✨ Features

- 🎯 **Differential Evolution (DE)** — popular evolutionary optimization method
- 🌐 **SHGO (Stochastic Hierarchical Global Optimization)** — hierarchical stochastic method for complex landscapes
- 📊 **Support for test functions**: GKLS, Grishagin, Hill, Shekel [Function Repository](https://github.com/OptimLLab/GCGen/tree/master)
- ⚡ **High performance** thanks to modern C++17
- 🧪 **Full test coverage** with GoogleTest
- 🕒 **Function execution time measurement**
- 📈 **Visualization** of optimization process
- 🔧 **Easy integration** into existing projects

## 🚀 Quick Start

### 📋 Prerequisites

- **CMake** 3.31 or higher
- **C++17 Compiler** (GCC 11+, Clang 14+, MSVC 2022+)
- **Git** for cloning dependencies

### 📥 Installation and Build

```bash
# 1. Clone the repository with submodules
git clone --recursive https://github.com/Egor1dzeN/global-opt find_global_opt
cd find_global_opt

# 2. Create build directory
mkdir build && cd build

# 3. Configure project
cmake -DCMAKE_BUILD_TYPE=Release ..

# 4. Build project
cmake --build . --config Release --parallel

# 5. Run example
./find_global_opt
```

## 📁 Project Structure

```
find_global_opt/
├── 📁 include/                           # Library header files
│   ├── 📁 differential_evolution/        # 🎯 Differential Evolution algorithm
│   │   └── differential_evolution.h
│   ├── 📁 shgo/                          # 🌐 SHGO algorithm
│   │   ├── shgo.h                        # Main SHGO interface
│   │   ├── delaunay.h                    # Delaunay triangulation
│   │   ├── edge.h                        # Edge structure
│   │   ├── simplex.h                     # Simplex for local minimum search
│   │   ├── hooke_jeeves.h                # Hooke-Jeeves method
│   │   ├── tools.h                       # Helper tools
│   │   └── measuring_time.h              # ⏱️ Time measurement
│   └── measuring_time.h                  # ⏱️ Time measurement (global)
├── 📁 src/                               # Source files
│   ├── 📁 differential_evolution/        # DE implementation
│   │   └── differential_evolution.cpp
│   ├── 📁 shgo/                          # SHGO implementation
│   │   └── shgo.cpp
│   ├── measuring_time.cpp
│   └── python_bindings.cpp               # 🐍 Python bindings (pybind11)
├── 📁 example/                           # 📖 Usage examples
│   └── main.cpp
├── 📁 tests/                             # 🧪 Tests
│   ├── 📁 differential_evolution/        # DE tests
│   │   └── *test.cpp
│   └── 📁 shgo/                          # SHGO tests
│       └── *test.cpp
├── 📁 tools/                             # 🔧 Helper tools
├── 📁 visualization/                     # 📈 Algorithm visualization
│   ├── animation.py
│   ├── plot2D.py
│   ├── plot3D.py
│   ├── requirements.txt
│   └── functions/
├── 📁 3rd_party/                         # 📦 Dependencies
│   └── GCGen/                            # 📊 Test function generator
├── 📄 CMakeLists.txt                     # 🛠️ Project build
└── 📄 README.md                          # 📋 This documentation
```

## 💡 Usage

### Example 1: Differential Evolution (DE)

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

### Example 2: SHGO (Stochastic Hierarchical Global Optimization)

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
    std::cout << "Global minimum: " << tHillProblem.GetOptimumValue() << "\n";
    std::cout << "Call count: " << res.nfev << "\n";
    return 0;
}
```

## 🔄 Optimization Methods Comparison

| Characteristic | Differential Evolution | SHGO |
|---|---|---|
| **Type** | Evolutionary algorithm | Hierarchical stochastic |
| **Speed** | Fast | Slower, but more accurate |
| **Reliability** | Good | Very high |
| **Function Complexity** | Better for simple | Excellent for complex landscapes |
| **Recommended for** | Real-time tasks | Critical accuracy |

## 📊 Supported Test Functions

The library includes the **GCGen** test function generator from [Repository](https://github.com/OptimLLab/GCGen):

- **GKLS** functions (multidimensional) — complex multimodal functions
- **Grishagin** functions — well-studied test functions
- **Hill** functions — plateau functions
- **Shekel** functions — functions with sharp peaks

## 📊 Visualization Results

The library includes Python scripts for visualizing the optimization process:

```bash
cd visualization
python animation.py  # Generates animation of minimum search
```

Supported functions for visualization:
- Rosenbrock function
- Other 2D/3D functions

## 🧪 Running Tests

```bash
cd build
./find_global_opt_test  # Direct test executable run
```

Tests cover all supported test functions:
- GKLS test
- Grishagin test
- Hill test
- Shekel test

## 📦 Integration into Your Project

### Via CMake (recommended)

```cmake
# In your CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
    find_global_opt
    GIT_REPOSITORY https://github.com/Egor1dzeN/global-opt
    GIT_TAG main  # or specific version
)
FetchContent_MakeAvailable(find_global_opt)

# Link with your project
target_link_libraries(<your project name> PRIVATE find_global_opt)
target_include_directories(<your project name> PRIVATE ${find_global_opt_SOURCE_DIR}/include)
```

## ⚙️ Optimization Parameters

| Parameter | Description | Default |
|---|---|---|
| `maxIterations` | 🔄 Maximum number of iterations | User-defined |
| `populationSize` | 👥 Population size | 40 |
| `dimension` | 📐 Search space dimensionality | User-defined |
| `crossoverRate` | 🔀 Crossover probability | 0.9 |
| `differentialWeight` | ⚖️ Differential weight | 0.8 |
| `bounds` | 📏 Search bounds | User-defined |

## 📊 Optimization Results

### Differential Evolution Result

```cpp
std::pair<double, std::vector<double>> 
// first: minimum function value
// second: argument at which minimum is achieved
```

### SHGO Result

```cpp
struct OptimizeResult {
    double minimum_value;              // Found minimum value
    std::vector<double> minimizers;    // Argument of minimum
    int num_function_calls;            // Number of function calls
    bool success;                      // Whether optimization succeeded
    std::string message;               // Status message
}
```

## 🛠️ Development Build

```bash
# Build with debug information
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --parallel

# Build with sanitizers
cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZERS=ON ..
```

## 📄 License

This project is distributed under the MIT License. See [LICENSE](LICENSE) file for details.
