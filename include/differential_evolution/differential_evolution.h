//
// Created by egorm on 20-Oct-25.
//
#pragma once

#include "Shekel/ShekelProblem.hpp"
#include <functional>
#include "optimize_result.h"

struct DEOptions {
    int popsize = 15;
    double F = 0.8;
    double CR = 0.9;
    int random_seed = 42;

    int maxiter = 1000;
    double tol = 1e-6;

    DEOptions() = default;

    explicit DEOptions(int gens, int pop_size = 100, double f = 0.8, double cr = 0.9)
            : maxiter(gens), popsize(pop_size), F(f), CR(cr) {}

};

OptimizeResult
differential_evolution(const std::function<double(const std::vector<double> &)> &objFunc,
                       const std::vector<std::pair<double, double>> &bounds, const DEOptions& options = DEOptions());

