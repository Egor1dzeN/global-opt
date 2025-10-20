//
// Created by egorm on 20-Oct-25.
//
#include <GlobalMinimumShekel.h>
#include <Shekel/ShekelProblem.hpp>
#include <bits/stdc++.h>

const int FUNCTION_INDEX = 2;

bool compare(const double first, const double second, const double eps = 0.0001) {
    return (first - eps <= second && second <= first + eps);
}

int main() {
    TShekelProblem tShekelProblem(FUNCTION_INDEX);
    auto res = findGlobalMinimum(0, [&](const vector<double> &x) -> double {
        return tShekelProblem.ComputeFunction(x);
    });
    auto target = minShekel[FUNCTION_INDEX];
    double pred_output = res.first;
    double pred_input = res.second;

    if (compare(pred_input, target[1], 0.1) && compare(pred_output, target[0], 0.1)) {
        return 0;
    }
    return -1;
}