//
// Created by egorm on 01-Apr-26.
//

#ifndef FIND_GLOBAL_OPT_OPTIMIZE_RESULT_H
#define FIND_GLOBAL_OPT_OPTIMIZE_RESULT_H
#include <vector>
struct OptimizeResult{
    double fun;
    std::vector<double> x;
    int nfev;
};
#endif //FIND_GLOBAL_OPT_OPTIMIZE_RESULT_H
