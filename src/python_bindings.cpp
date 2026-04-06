#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "shgo/shgo.h"
#include "differential_evolution/differential_evolution.h"

namespace py = pybind11;

PYBIND11_MODULE(global_opt_py, m) {
    m.doc() = "Global optimization module";

    // Bind OptimizeResult struct
    py::class_<OptimizeResult>(m, "OptimizeResult")
        .def(py::init<>())
        .def_readwrite("fun", &OptimizeResult::fun, "Function value at the solution")
        .def_readwrite("x", &OptimizeResult::x, "Solution point")
        .def_readwrite("nfev", &OptimizeResult::nfev, "Number of function evaluations")
        .def("__repr__", [](const OptimizeResult &result) {
            return "OptimizeResult(fun=" + std::to_string(result.fun) +
                   ", x=[...], nfev=" + std::to_string(result.nfev) + ")";
        });

    // Bind shgo function
    m.def("shgo",
          [](py::object func, const std::vector<std::pair<double, double>> &bounds, int count_point = 100) {
              auto cpp_func = [func](const std::vector<double> &x) -> double {
                  return func(x).cast<double>();
              };
              return shgo(cpp_func, bounds, count_point);
          },
          py::arg("func"),
          py::arg("bounds"),
          py::arg("count_point") = 100,
          R"(
          Minimize a function using Simplicial Homology Global Optimization.

          Parameters
          ----------
          func : callable
              The objective function to be minimized. Must take a list/array of float values
              and return a single float value.
          bounds : list of tuples
              Bounds for variables as a list of (lower, upper) tuples.
              Example: [(0, 5), (-5, 5)] for 2D problem
          count_point : int, optional
              Number of sample points for initial triangulation. Default is 100.

          Returns
          -------
          OptimizeResult
              Object with attributes:
              - fun: float - function value at solution
              - x: list - solution point
              - nfev: int - number of function evaluations
          )");
    // Bind shgo function
    m.def("differential_evolution",
          [](py::object func, const std::vector<std::pair<double, double>> &bounds, int count_point = 100) {
              auto cpp_func = [func](const std::vector<double> &x) -> double {
                  return func(x).cast<double>();
              };
              return differential_evolution(cpp_func, bounds, count_point);
          },
          py::arg("func"),
          py::arg("bounds"),
          py::arg("count_point") = 100,
          R"(
          Minimize a function using Differential Evolution.

          Parameters
          ----------
          func : callable
              The objective function to be minimized. Must take a list/array of float values
              and return a single float value.
          bounds : list of tuples
              Bounds for variables as a list of (lower, upper) tuples.
              Example: [(0, 5), (-5, 5)] for 2D problem
          count_point : int, optional
              Number of sample points for initial triangulation. Default is 100.

          Returns
          -------
          OptimizeResult
              Object with attributes:
              - fun: float - function value at solution
              - x: list - solution point
              - nfev: int - number of function evaluations
          )");
}
