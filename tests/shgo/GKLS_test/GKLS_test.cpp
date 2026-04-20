//
// Created by egorm on 10-Nov-25.
//
#include "gtest/gtest.h"
#include "GKLS/GKLSProblem.hpp"
#include "shgo/shgo.h"
#include "measuring_time.h"
#include "create_bounds.h"

double calculateTGKLS_shgo(TGKLSProblem &tgklsProblem, int count_point = 100) {
    std::vector<std::pair<double, double>> bounds = CreateBounds(tgklsProblem);
    auto res = shgo([&](const std::vector<double> &x) -> double {
        return tgklsProblem.ComputeFunction(x);
    }, bounds, count_point);
    return res.fun;
}

//TEST(GKLS_Test, BasicTest_1dim_1) {
//    int functionId = 1;
//    int input_size = 1;
//    TGKLSProblem tgklsProblem(functionId, input_size);
//    double res = calculateTGKLS_shgo(tgklsProblem);
//    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
//}

TEST(GKLS_Test, BasicTest_2dim) {
    int functionId = 1;
    int input_size = 2;
    TGKLSProblem tgklsProblem(functionId, input_size);
    double res = calculateTGKLS_shgo(tgklsProblem, 10);
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}

TEST(GKLS_Test, BasicTest_3dim) {
    int functionId = 1;
    int input_size = 3;
    TGKLSProblem tgklsProblem(functionId, input_size);
    double res = calculateTGKLS_shgo(tgklsProblem, 7);
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}