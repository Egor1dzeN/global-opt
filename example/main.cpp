//
// Created by egorm on 20-Oct-25.
//
#include <GlobalMinimumShekel.h>
#include "Hill/HillProblem.hpp"
#include <Shekel/ShekelProblem.hpp>
#include <bits/stdc++.h>

const int FUNCTION_INDEX = 0;

bool compare(const double first, const double second, const double eps = 0.0001) {
    return (first - eps <= second && second <= first + eps);
}

int main() {
    TShekelProblem tShekelProblem(FUNCTION_INDEX);
    auto shekel_res = findGlobalMinimum(0, [&](const vector<double> &x) -> double {
        return tShekelProblem.ComputeFunction(x);
    });
    auto shekel_target = minShekel[FUNCTION_INDEX];
    double shekel_pred_output = shekel_res.first;
    double shekel_pred_input = shekel_res.second;

    if (compare(shekel_pred_input, shekel_target[1], 0.1) && compare(shekel_pred_output, shekel_target[0], 0.1)) {
        std::cout<<"(ShekelProblem) The result is within the normal range\n";
    }else{
        std::cout<<"(ShekelProblem) The result isn't within the normal range\n";
    }

    THillProblem tHillProblem(FUNCTION_INDEX);
    auto hill_res = findGlobalMinimum(0, [&](const vector<double> &x) -> double {
        return tHillProblem.ComputeFunction(x);
    });
    auto hill_target = minHill[FUNCTION_INDEX];
    double hill_pred_output = hill_res.first;
    double hill_pred_input = hill_res.second;
    if (compare(hill_pred_output, hill_target[0], 0.1)) {
        std::cout<<"(HillProblem) The result is within the normal range\n";
    }else{
        std::cout<<"(HillProblem) The result isn't within the normal range\n";
    }
    return -1;
}