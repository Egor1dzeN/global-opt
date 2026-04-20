//
// Created by egorm on 10-Nov-25.
//
#include "gtest/gtest.h"
#include "GKLS/GKLSProblem.hpp"
#include "differential_evolution/differential_evolution.h"
#include "measuring_time.h"
#include "create_bounds.h"

double calculateTGKLS_DE(TGKLSProblem &tgklsProblem) {
    std::vector<std::pair<double, double>> bounds = CreateBounds(tgklsProblem);
    for (auto [f, s] : bounds){
        std::cout<<f<<" "<<s<<"\n";
    }
    auto res = differential_evolution([&](const std::vector<double> &x) -> double {
        return tgklsProblem.ComputeFunction(x);
    }, bounds, 1000);
    return res.fun;
}

TEST(GKLS_de_Test, BasicTest_2dim_1) {
    int functionId = 1;
    int input_size = 2;
    TGKLSProblem tgklsProblem(functionId, input_size);
    double res = calculateTGKLS_DE(tgklsProblem);
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}

TEST(GKLS_de_Test, BasicTest_3dim) {
    int functionId = 1;
    int input_size = 3;
    TGKLSProblem tgklsProblem(functionId, input_size);
    double res = calculateTGKLS_DE(tgklsProblem);
    for (int i = 0;i<tgklsProblem.GetOptimumPoint().size();++i){
        std::cout<<tgklsProblem.GetOptimumPoint()[i]<<" ";
    }
    std::cout<<std::endl;
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}

TEST(GKLS_de_Test, BasicTest_5dim) {
    int functionId = 1;
    int input_size = 5;
    TGKLSProblem tgklsProblem(functionId, input_size);
    for (int i = 0;i<tgklsProblem.GetOptimumPoint().size();++i){
        std::cout<<tgklsProblem.GetOptimumPoint()[i]<<" ";
    }
    std::cout<<std::endl;
    double res = calculateTGKLS_DE(tgklsProblem);
    EXPECT_NEAR(res, tgklsProblem.GetOptimumValue(), 0.1);
}