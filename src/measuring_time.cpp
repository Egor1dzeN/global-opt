//
// Created by egorm on 28-Oct-25.
//
#include "measuring_time.h"
#include "DE_finding_global_min.h"

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    if (vec.empty()) {
        os << "{}";
    } else if (vec.size() == 1) {
        os << vec[0];
    } else {
        os << "{";
        for (size_t i = 0; i < vec.size(); ++i) {
            os << vec[i];
            if (i != vec.size() - 1) {
                os << ", ";
            }
        }
        os << "}";
    }
    return os;
}

TResult
runSingleTest(IOptProblem &iOptProblem, int function_index, const std::string &function_name, size_t input_size) {
    auto start_time = std::chrono::high_resolution_clock::now();

    auto res = findGlobalMinimum([&](const std::vector<double> &x) -> double {
        return iOptProblem.ComputeFunction(x);
    }, input_size);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    double pred_output = res.first;
    std::vector<double> pred_input = res.second;

    double correct_output = iOptProblem.GetOptimumValue();
    std::vector<double> correct_input = iOptProblem.GetOptimumPoint();

    TResult result;
    result.function_index = function_index;
    result.function_name = function_name;
    result.predicted_output = pred_output;
    result.correct_output = correct_output;
    result.output_deviation = std::abs(pred_output - correct_output);
    result.predicted_input = pred_input;
    result.correct_input = correct_input;
    result.execution_time_ms = double(duration.count()) / 1000.0; // преобразуем в миллисекунды
    result.success = compare(pred_output, correct_output, 0.1);

    return result;
}

void writeResultsToCSV(const std::vector<std::pair<TResult, int>> &all_results, const std::string &filename,
                       int lunch_count) {
    std::ofstream file(filename);

    // Header CSV
    file << "function_id,function_name,predicted_output,correct_output,"
         << "output_deviation,predicted_input,correct_input,execution_time_ms,success\n";

    for (const auto &result_pair: all_results) {
        TResult tResult = result_pair.first;
        int count_success = result_pair.second;
        file << tResult.function_index << ","
             << tResult.function_name << ","
             << tResult.predicted_output << ","
             << tResult.correct_output << ","
             << tResult.output_deviation << ",\""
             << tResult.predicted_input << "\",\""
             << tResult.correct_input << "\","
             << tResult.execution_time_ms << ","
             << count_success << "/" << lunch_count << "\n";

    }

    file.close();
}

bool compare(double first, double second, double eps = 0.0001) {
    return (std::abs(first - second) <= eps);
}

