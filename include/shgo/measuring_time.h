//
// Created by egorm on 28-Oct-25.
//
#pragma once

#include "IOptProblem.hpp"
#include "hooke_jeeves.h"

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
    int count_generation;
};

TResult
runSingleTest(IOptProblem &iOptProblem, int function_index, const std::vector<pdd> &bounds,
              const std::string &function_name, size_t input_size,
              int count_generation);

void writeResultsToCSV(const std::vector<std::pair<TResult, int>> &all_results, const std::string &filename,
                       int lunch_count);

bool compare(double first, double second, double eps);

