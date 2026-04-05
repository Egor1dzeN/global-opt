//
// Created by egorm on 10-Nov-25.
//
#include "gtest/gtest.h"
#include "create_bounds.h"
#include "Grishagin/grishagin_function.hpp"
#include "shgo/shgo.h"
#include "measuring_time.h"

double calculateTGrishagin_shgo(TGrishaginProblem &grishaginProblem, int count_point=20) {
    std::vector<std::pair<double, double>> bounds = CreateBounds(grishaginProblem);
    auto res = shgo([&](const std::vector<double> &x) -> double {
        return grishaginProblem.ComputeFunction(x);
    }, bounds, count_point);
    return res.fun;
}

TEST(Grishagin_Test, BasicTest_1) {
    int functionId = 1;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin_shgo(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}

TEST(Grishagin_Test, BasicTest_2) {
    int functionId = 3;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin_shgo(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}

TEST(Grishagin_Test, BasicTest_3) {
    int functionId = 5;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin_shgo(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}
TEST(Grishagin_Test, BasicTest_4) {
    int functionId = 100;
    TGrishaginProblem tGrishaginProblem(functionId);
    double res = calculateTGrishagin_shgo(tGrishaginProblem);
    EXPECT_NEAR(res, tGrishaginProblem.GetOptimumValue(), 0.1);
}