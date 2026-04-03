//
// Created by egorm on 01-Apr-26.
//

#ifndef FIND_GLOBAL_OPT_CREATE_BOUNDS_H
#define FIND_GLOBAL_OPT_CREATE_BOUNDS_H

#include <iostream>
#include "IOptProblem.hpp"

inline std::vector<std::pair<double, double>> CreateBounds(IOptProblem &iOptProblem) {
    std::vector<double> lb;
    std::vector<double> ub;
    iOptProblem.GetBounds(lb, ub);
    std::vector<std::pair<double, double>> result(lb.size());
    for (int i = 0; i < lb.size(); ++i) {
        result[i] = std::make_pair(lb[i], ub[i]);
    }
    return result;
}

#endif //FIND_GLOBAL_OPT_CREATE_BOUNDS_H
