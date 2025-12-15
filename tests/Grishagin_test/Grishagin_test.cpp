//
// Created by egorm on 10-Nov-25.
//
#include "gtest/gtest.h"
#include "Grishagin/grishagin_function.hpp"
#include "DE_finding_global_min.h"
#include "measuring_time.h"

double calculateTGrishagin(TGrishaginProblem &grishaginProblem) {
    auto [optimal_value, _] = findGlobalMinimum([&](const std::vector<double> &x) -> double {
        return grishaginProblem.ComputeFunction(x);
    }, grishaginProblem.GetDimension(), 0.0, 1.0, 300);
    return optimal_value;
}

TEST(Grishagin_Test, BasicTest_1) {
    int functionId = 1;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}

TEST(Grishagin_Test, BasicTest_2) {
    int functionId = 3;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}

TEST(Grishagin_Test, BasicTest_3) {
    int functionId = 5;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}
TEST(Grishagin_Test, BasicTest_4) {
    int functionId = 100;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}