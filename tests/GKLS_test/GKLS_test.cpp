//
// Created by egorm on 10-Nov-25.
//
#include "gtest/gtest.h"
#include "GKLS/GKLSProblem.hpp"
#include "DE_finding_global_min.h"
#include "measuring_time.h"
#include "Grishagin/grishagin_function.hpp"

double calculateTGKLS(TGKLSProblem &tgklsProblem) {
    auto [optimal_value, _] = findGlobalMinimum([&](const std::vector<double> &x) -> double {
        return tgklsProblem.ComputeFunction(x);
    }, tgklsProblem.GetDimension(), -3.0, 3.0);
    return optimal_value;
}

TEST(GKLS_Test, BasicTest_2dim) {
    int functionId = 1;
    int input_size = 2;
    TGKLSProblem tgklsProblem(functionId, input_size);
    double res = calculateTGKLS(tgklsProblem);
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}

TEST(GKLS_Test, BasicTest_3dim) {
    int functionId = 1;
    int input_size = 3;
    TGKLSProblem tgklsProblem(functionId, input_size);
    double res = calculateTGKLS(tgklsProblem);
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}

TEST(GKLS_Test, BasicTest_5dim) {
    int functionId = 1;
    int input_size = 5;
    TGKLSProblem tgklsProblem(functionId, input_size);
    for (auto el: tgklsProblem.GetMaxPoint())
        std::cout << el << "\n";
    double res = calculateTGKLS(tgklsProblem);
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}