//
// Created by egorm on 10-Nov-25.
//

//
// Created by egorm on 10-Nov-25.
//
#include "gtest/gtest.h"
#include "Hill/HillProblem.hpp"
#include "DE_finding_global_min.h"
#include "measuring_time.h"

double calculateTGrishagin(THillProblem &hillProblem) {
    auto [optimal_value, _] = findGlobalMinimum([&](const std::vector<double> &x) -> double {
        return hillProblem.ComputeFunction(x);
    }, hillProblem.GetDimension(), 0.0, 1.0, 300);
    return optimal_value;
}

TEST(Hill_Test, BasicTest_1) {
    int functionId = 1;
    THillProblem problem(functionId);
    double res = calculateTGrishagin(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Hill_Test, BasicTest_2) {
    int functionId = 3;
    THillProblem problem(functionId);
    double res = calculateTGrishagin(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Hill_Test, BasicTest_3) {
    int functionId = 5;
    THillProblem problem(functionId);
    double res = calculateTGrishagin(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Hill_Test, BasicTest_4) {
    int functionId = 100;
    THillProblem problem(functionId);
    double res = calculateTGrishagin(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}
TEST(Hill_Test, BasicTest_5) {
    int functionId = NUM_HILL_PROBLEMS-1;
    THillProblem problem(functionId);
    double res = calculateTGrishagin(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}