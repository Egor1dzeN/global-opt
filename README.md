# Find Global Opt

[![CMake](https://img.shields.io/badge/CMake-3.31+-brightgreen.svg)](https://cmake.org/)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Tests](https://img.shields.io/badge/tests-GoogleTest-orange.svg)](https://github.com/google/googletest)
[![License](https://img.shields.io/badge/license-MIT-yellow.svg)](LICENSE)

Find Global Opt is a C++17 library for finding the global minimum of multidimensional functions
over a box-constrained domain.

## Features

- **Differential Evolution (DE)**: population-based evolutionary method.
- **SHGO (Simplicial Homology Global Optimization)**: Halton sampling, Delaunay triangulation and
  local refinement with the Hooke-Jeeves method.
- **Test problems**: GKLS, Grishagin, Hill and Shekel families from
  [GCGen](https://github.com/OptimLLab/GCGen), plus 54 classic benchmark functions.
- **Python bindings** (pybind11) and visualization scripts.
- **Benchmark** executable for measuring time and solution quality (see [benchmark/README.md](benchmark/README.md)).

## Requirements

- CMake 3.31 or newer
- C++17 compiler (GCC 11+, Clang 14+, MSVC 2022+)
- Git (the GCGen dependency is a submodule)

## Build

```bash
git clone --recursive https://github.com/Egor1dzeN/global-opt find_global_opt
cd find_global_opt

cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build --config Release --parallel
```

Targets:

| Target                      | Description                       |
|-----------------------------|-----------------------------------|
| `find_global_opt`           | Example executable                |
| `find_global_opt_test`      | GoogleTest suite                  |
| `find_global_opt_benchmark` | Benchmark of DE and SHGO          |
| `global_opt_py`             | Python module (pybind11)          |

## Project structure

```
find_global_opt/
├── include/
│   ├── differential_evolution/   Differential Evolution interface
│   ├── shgo/                     SHGO interface, Delaunay triangulation, Hooke-Jeeves
│   ├── test_functions/           Classic benchmark functions
│   ├── optimize_result.h         Result structure
│   └── measuring_time.h          Execution time measurement
├── src/                          Implementations and Python bindings
├── tests/                        GoogleTest suites
├── benchmark/                    Benchmark executable and reference results
├── example/                      Usage example
├── visualization/                Python visualization scripts
├── 3rd_party/GCGen/              Test problem generator (submodule)
└── CMakeLists.txt
```

## Usage

Both methods take an objective function and a vector of `(lower, upper)` bounds, one pair per
dimension, and return an `OptimizeResult`.

### Differential Evolution

```cpp
#include "Hill/HillProblem.hpp"  // GCGen
#include "differential_evolution/differential_evolution.h"
#include <iostream>

int main() {
    THillProblem problem;
    DEOptions options;
    options.maxiter = 1000;

    auto res = differential_evolution([&](const std::vector<double> &x) {
        return problem.ComputeFunction(x);
    }, {{-1., 1.}}, options);

    std::cout << "f(x*) = " << res.fun << ", nfev = " << res.nfev << "\n";
    std::cout << "known optimum = " << problem.GetOptimumValue() << "\n";
}
```

`DEOptions` fields:

| Field         | Description                         | Default |
|---------------|-------------------------------------|---------|
| `popsize`     | Population size                     | 15      |
| `F`           | Differential weight                 | 0.8     |
| `CR`          | Crossover probability               | 0.9     |
| `maxiter`     | Maximum number of generations       | 1000    |
| `tol`         | Convergence tolerance               | 1e-6    |
| `random_seed` | Seed of the random number generator | 42      |

### SHGO

```cpp
#include "Hill/HillProblem.hpp"
#include "shgo/shgo.h"
#include <iostream>

int main() {
    THillProblem problem;
    auto res = shgo([&](const std::vector<double> &x) {
        return problem.ComputeFunction(x);
    }, {{-1., 1.}}, /*count_point=*/100);

    std::cout << "f(x*) = " << res.fun << ", nfev = " << res.nfev << "\n";
}
```

`count_point` is the number of initial sample points (default 100).

### Result

```cpp
struct OptimizeResult {
    double fun;              // Minimum value found
    std::vector<double> x;   // Point at which it was found
    int nfev;                // Number of objective function evaluations
};
```

## Test functions

### GCGen

The [GCGen](https://github.com/OptimLLab/GCGen) generator provides the GKLS (multidimensional),
Grishagin (2D), Hill (1D) and Shekel (1D) problem families.

### Classic functions

`include/test_functions/classic_functions.h` defines 54 functions with known bounds and global minimum.

- **Arbitrary dimension (25):** Sphere, Rastrigin, Ackley, Rosenbrock, Griewank, Schwefel, Levy, Zakharov,
  Styblinski-Tang, Dixon-Price, Sum Squares, Rotated Hyper-Ellipsoid, Sum of Different Powers, Trid,
  Powell, Alpine N.1, Salomon, Qing, Exponential, Bent Cigar, Discus, Schwefel 2.22, Step, Quartic, Michalewicz.
- **Fixed dimension (29):** Beale, Booth, Matyas, Himmelblau, Three-Hump Camel, Six-Hump Camel, Easom,
  Goldstein-Price, Branin, Bukin N.6, Cross-in-Tray, Drop-Wave, Eggholder, Holder Table, Levy N.13,
  Schaffer N.2, Schaffer N.4, McCormick, Shubert, Bohachevsky N.1, Leon, Bird, De Jong N.5 (2D);
  Hartmann 3D; Colville, Shekel 5/7/10 (4D); Hartmann 6D.

```cpp
#include "test_functions/classic_functions.h"

auto tf = test_functions::rastrigin(5);  // or test_functions::get_function("rastrigin", 5)
auto res = shgo(tf.function, tf.bounds);
std::cout << res.fun << " vs " << tf.f_opt << "\n";

for (const auto &f : test_functions::functions_for_dimension(2)) { /* all 2D functions */ }
```

## Tests

```bash
./build/find_global_opt_test
```

The suite runs both methods on the GKLS, Grishagin, Hill and Shekel families and checks the classic functions.

## Visualization

```bash
cd visualization
pip install -r requirements.txt
python animation.py           # GIF animation of Differential Evolution
python window/main.py         # Interactive window (requires the global_opt_py module)
```

## License

MIT. See [LICENSE](LICENSE).
