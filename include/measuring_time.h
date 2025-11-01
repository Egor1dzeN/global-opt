//
// Created by egorm on 28-Oct-25.
//
#include <bits/stdc++.h>
#include "IOptProblem.hpp"

#ifndef UNTITLED5_MEASURING_TIME_H
#define UNTITLED5_MEASURING_TIME_H

struct TResult {
    int function_index;
    std::string function_name;
    double predicted_output;
    double correct_output;
    double output_deviation;
    std::vector<double> predicted_input;
    std::vector<double> correct_input;
    double execution_time_ms;
    bool success;
};

TResult runSingleTest(IOptProblem &iOptProblem, int function_index, const std::string &function_name, size_t input_size);

void writeResultsToCSV(const std::vector<std::vector<TResult>> &all_results, const std::string &filename);

bool compare(double first, double second, double eps);

#endif //UNTITLED5_MEASURING_TIME_H
