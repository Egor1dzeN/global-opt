//
// Created by egorm on 20-Oct-25.
//
#include <Shekel/ShekelProblem.hpp>
#include <functional>
#include <bits/stdc++.h>

#ifndef UNTITLED5_GLOBALMINIMUMSHEKEL_H
#define UNTITLED5_GLOBALMINIMUMSHEKEL_H

std::pair<double, double>
findGlobalMinimum(const std::function<double(const std::vector<double> &)> &objFunc, size_t input_size);

#endif //UNTITLED5_GLOBALMINIMUMSHEKEL_H
