//
// Created by egorm on 3/29/2026.
//

#ifndef SHGO2_SHGO_H
#define SHGO2_SHGO_H


#include <functional>
#include "Shekel/ShekelProblem.hpp"
#include "delaunay.h"
#include "hooke_jeeves.h"
#include "optimize_result.h"

OptimizeResult
shgo(const std::function<double(const std::vector<double> &)> &, const std::vector<std::pair<double, double>> &);

#endif //SHGO2_SHGO_H
