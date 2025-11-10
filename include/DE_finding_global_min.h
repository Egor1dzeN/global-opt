//
// Created by egorm on 20-Oct-25.
//
#include <Shekel/ShekelProblem.hpp>
#include <functional>

#ifndef UNTITLED5_GLOBALMINIMUMSHEKEL_H
#define UNTITLED5_GLOBALMINIMUMSHEKEL_H

std::pair<double, std::vector<double>>
findGlobalMinimum(const std::function<double(const std::vector<double> &)> &objFunc, size_t input_size,
                  double min_y = -1.0, double max_y = 1.0, int count_generation = 200);

#endif //UNTITLED5_GLOBALMINIMUMSHEKEL_H
