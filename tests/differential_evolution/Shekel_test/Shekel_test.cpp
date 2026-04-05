//
// Created by egorm on 10-Nov-25.
//

#include <iostream>
#include "gtest/gtest.h"
#include "create_bounds.h"
#include "Shekel/ShekelProblem.hpp"
#include "differential_evolution/differential_evolution.h"
#include "measuring_time.h"

double calculateTShekel_DE(TShekelProblem &hillProblem) {
    std::vector<std::pair<double, double>> bounds = CreateBounds(hillProblem);
    auto res = differential_evolution([&](const std::vector<double> &x) -> double {
        return hillProblem.ComputeFunction(x);
    }, bounds, 300);
    return res.fun;
}

TEST(Shekel_de_Test, BasicTest_1) {
    int functionId = 1;
    TShekelProblem problem(functionId);
    double res = calculateTShekel_DE(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_de_Test, BasicTest_2) {
    int functionId = 2;
    TShekelProblem problem(functionId);
    double res = calculateTShekel_DE(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_de_Test, BasicTest_3) {
    int functionId = 3;
    TShekelProblem problem(functionId);
    double res = calculateTShekel_DE(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_de_Test, BasicTest_4) {
    int functionId = 4;
    TShekelProblem problem(functionId);
    double res = calculateTShekel_DE(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_de_Test, BasicTest_5) {
    int functionId = 5;
    TShekelProblem problem(functionId);
    double res = calculateTShekel_DE(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_de_Test, BasicTest_100) {
    int functionId = 100;
    TShekelProblem problem(functionId);
    double res = calculateTShekel_DE(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}

TEST(Shekel_de_Test, BasicTest_max) {
    int functionId = NUM_SHEKEL_PROBLEMS - 1;
    TShekelProblem problem(functionId);
    double res = calculateTShekel_DE(problem);
    EXPECT_NEAR(res, problem.GetOptimumValue(), 0.1);
}