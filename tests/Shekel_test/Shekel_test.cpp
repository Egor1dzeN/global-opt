//
// Created by egorm on 10-Nov-25.
//

#include "gtest/gtest.h"
#include "Shekel/ShekelProblem.hpp"
#include "DE_finding_global_min.h"
#include "measuring_time.h"

double calculateTShekel(TShekelProblem &hillProblem) {
    auto [optimal_value, _] = findGlobalMinimum([&](const std::vector<double> &x) -> double {
        return hillProblem.ComputeFunction(x);
    }, hillProblem.GetDimension(), -10.0, 10.0, 300);
    return optimal_value;
}

TEST(Shekel_Test, BasicTest_1) {
    int functionId = 1;
    TShekelProblem problem(functionId);
    double res = calculateTShekel(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_Test, BasicTest_2) {
    int functionId = 2;
    TShekelProblem problem(functionId);
    double res = calculateTShekel(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_Test, BasicTest_3) {
    int functionId = 3;
    TShekelProblem problem(functionId);
    double res = calculateTShekel(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_Test, BasicTest_4) {
    int functionId = 4;
    TShekelProblem problem(functionId);
    double res = calculateTShekel(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_Test, BasicTest_5) {
    int functionId = 5;
    TShekelProblem problem(functionId);
    double res = calculateTShekel(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_Test, BasicTest_100) {
    int functionId = 100;
    TShekelProblem problem(functionId);
    double res = calculateTShekel(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_Test, BasicTest_max) {
    int functionId = NUM_SHEKEL_PROBLEMS - 1;
    TShekelProblem problem(functionId);
    double res = calculateTShekel(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}