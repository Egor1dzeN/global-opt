//
// Created by egorm on 20-Oct-25.
//
#pragma once

#include "Shekel/ShekelProblem.hpp"
#include <functional>
#include "optimize_result.h"

OptimizeResult
differential_evolution(const std::function<double(const std::vector<double> &)> &objFunc,
                       const std::vector<std::pair<double, double>> &bounds, int count_generation=200);

