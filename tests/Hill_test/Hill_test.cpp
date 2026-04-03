//
// Created by egorm on 10-Nov-25.
//

#include "gtest/gtest.h"
#include "create_bounds.h"
#include "Hill/HillProblem.hpp"
#include "differential_evolution/differential_evolution.h"
#include "measuring_time.h"

double calculateTGrishagin(THillProblem &hillProblem) {
    std::vector<std::pair<double, double>> bounds = CreateBounds(hillProblem);
    auto res = differential_evolution([&](const std::vector<double> &x) -> double {
        return hillProblem.ComputeFunction(x);
    }, bounds, 300);
    return res.fun;
}

TEST(Hill_Test, BasicTest_1
) {
int functionId = 1;
THillProblem problem(functionId);
double res = calculateTGrishagin(problem);
EXPECT_NEAR(res, problem
.

GetOptimumValue(),

0.1);
}

TEST(Hill_Test, BasicTest_2
) {
int functionId = 2;
THillProblem problem(functionId);
double res = calculateTGrishagin(problem);
EXPECT_NEAR(res, problem
.

GetOptimumValue(),

0.1);
}

TEST(Hill_Test, BasicTest_3
) {
int functionId = 3;
THillProblem problem(functionId);
double res = calculateTGrishagin(problem);
EXPECT_NEAR(res, problem
.

GetOptimumValue(),

0.1);
}

TEST(Hill_Test, BasicTest_4
) {
int functionId = 4;
THillProblem problem(functionId);
double res = calculateTGrishagin(problem);
EXPECT_NEAR(res, problem
.

GetOptimumValue(),

0.1);
}

TEST(Hill_Test, BasicTest_5
) {
int functionId = 5;
THillProblem problem(functionId);
double res = calculateTGrishagin(problem);
EXPECT_NEAR(res, problem
.

GetOptimumValue(),

0.1);
}

TEST(Hill_Test, BasicTest_100
) {
int functionId = 100;
THillProblem problem(functionId);
double res = calculateTGrishagin(problem);
EXPECT_NEAR(res, problem
.

GetOptimumValue(),

0.1);
}

TEST(Hill_Test, BasicTest_max
) {
int functionId = NUM_HILL_PROBLEMS - 1;
THillProblem problem(functionId);
double res = calculateTGrishagin(problem);
EXPECT_NEAR(res, problem
.

GetOptimumValue(),

0.1);
}