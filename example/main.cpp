//
// Created by egorm on 20-Oct-25.
//

#include <DE_finding_global_min.h>
#include <Shekel/ShekelProblem.hpp>
#include <Hill/HillProblem.hpp>
#include <Grishagin/GrishaginConstrainedProblem.hpp>
#include <Grishagin/grishagin_function.hpp>
#include <GKLS/GKLSProblem.hpp>
#include <bits/stdc++.h>
#include "measuring_time.h"
#include <cxxabi.h>

const int NUMBER_LOOP = 10;

// Convert from mangled to demangled name of class
std::string demangle(const char *mangled_name) {
    int status = 0;
    char *demangled = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);

    if (status == 0) {
        std::string result(demangled);
        std::free(demangled);
        return result;
    }

    return mangled_name;
}

std::pair<TResult, int> avgTResult(const std::vector<TResult> &input) {
    TResult res = input[0];
    double prediction_output_sum = 0.0;
    double output_deviation_sum = 0.0;
    double execution_time_ms_sum = 0.0;
    int count_success = 0;

    for (const auto &tResult: input) {
        prediction_output_sum += tResult.predicted_output;
        output_deviation_sum += tResult.output_deviation;
        execution_time_ms_sum += tResult.execution_time_ms;
        count_success += tResult.success ? 1 : 0;
    }

    res.predicted_output = prediction_output_sum / static_cast<double>(input.size());
    res.output_deviation = output_deviation_sum / static_cast<double>(input.size());
    res.execution_time_ms = execution_time_ms_sum / static_cast<double>(input.size());

    return std::make_pair(res, count_success);
}

template<typename T>
std::enable_if_t<std::is_base_of_v<IOptProblem, T>>
measureTime(int startFunctionID = 1, int endFunctionID = 101, int run_count = 10, int input_size = 1) {
    std::vector<std::pair<TResult, int>> result_list;

    // Determine input size based on problem type
    if constexpr (std::is_same_v<T, TGrishaginProblem>) {
        input_size = 2;
    }

    for (int functionId = startFunctionID; functionId < endFunctionID; ++functionId) {
        // Create problem instance with appropriate constructor
        T problem = [&]() {
            if constexpr (std::is_same_v<T, TGKLSProblem>) {
                if (input_size <= 1) {
                    std::cerr << "Dimension cannot be less than or equal to 1!\n";
                    throw std::invalid_argument("Invalid dimension");
                }
                return T(functionId, input_size);
            } else {
                return T(functionId);
            }
        }();

        std::vector<TResult> results(run_count);

        // Run multiple tests for the same problem
        for (int j = 0; j < run_count; ++j) {
            TResult res = runSingleTest(problem, functionId,
                                        demangle(typeid(problem).name()),
                                        input_size);
            results[j] = res;
        }

        auto avg_pair = avgTResult(results);
        result_list.push_back(avg_pair);

        // Progress indicator
        if (functionId % 10 == 0) {
            std::cout << "Progress: " << functionId << "/"
                      << (endFunctionID - startFunctionID) << "\n";
        }
    }

    // Write final results to CSV
    std::string filename = demangle(typeid(T).name()) + "_test_itog.csv";
    writeResultsToCSV(result_list, filename, NUMBER_LOOP);
}

int main() {
    using T = THillProblem;
    measureTime<T>(1, 101, NUMBER_LOOP, 2);
    return 0;
}