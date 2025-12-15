//
// Created by egorm on 20-Oct-25.
//
#pragma once

#include <Shekel/ShekelProblem.hpp>
#include <functional>

std::pair<double, std::vector<double>>
findGlobalMinimum(const std::function<double(const std::vector<double> &)> &objFunc, size_t input_size,
                  double min_y = -1.0, double max_y = 1.0, int count_generation = 200);

