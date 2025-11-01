//
// Created by egorm on 28-Oct-25.
//
#include "measuring_time.h"
#include "DE_finding_global_min.h"

TResult
runSingleTest(IOptProblem &iOptProblem, int function_index, const std::string &function_name, size_t input_size) {
    auto start_time = std::chrono::high_resolution_clock::now();

    auto res = findGlobalMinimum([&](const std::vector<double> &x) -> double {
        return iOptProblem.ComputeFunction(x);
    }, input_size);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    double pred_output = res.first;
    double pred_input = res.second;

    double correct_output = iOptProblem.GetOptimumValue();
    double correct_input = iOptProblem.GetOptimumPoint()[0];

    TResult result;
    result.function_index = function_index;
    result.function_name = function_name;
    result.predicted_output = pred_output;
    result.correct_output = correct_output;
    result.output_deviation = std::abs(pred_output - correct_output);
    result.predicted_input = pred_input;
    result.correct_input = correct_input;
    result.execution_time_ms = duration.count() / 1000.0; // преобразуем в миллисекунды
    result.success = compare(pred_output, correct_output, 0.1);

    return result;
}

void writeResultsToCSV(const std::vector<std::vector<TResult>> &all_results, const std::string &filename) {
    std::ofstream file(filename);

    // Header CSV
    file << "function_id,function_name,test_run,predicted_output,correct_output,"
         << "output_deviation,predicted_input,correct_input,execution_time_ms,success\n";

    for (const auto &all_result: all_results) {
        for (size_t test_run = 0; test_run < all_result.size(); ++test_run) {
            const auto &result = all_result[test_run];

            file << result.function_index << ","
                 << result.function_name << ","
                 << (test_run + 1) << ","
                 << result.predicted_output << ","
                 << result.correct_output << ","
                 << result.output_deviation << ","
                 << result.predicted_input << ","
                 << result.correct_input << ","
                 << result.execution_time_ms << ","
                 << (result.success ? "true" : "false") << "\n";
        }
    }

    file.close();
}

bool compare(double first, double second, double eps = 0.0001) {
    return (std::abs(first - second) <= eps);
}

