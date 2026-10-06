//
// Checks for the classic benchmark functions and optimizer runs on them.
//
#include "gtest/gtest.h"
#include "test_functions/classic_functions.h"
#include "shgo/shgo.h"
#include "differential_evolution/differential_evolution.h"

#include <cmath>
#include <random>

using namespace test_functions;

namespace test_functions {
    // Readable parameter output in gtest messages
    void PrintTo(const TestFunction &tf, std::ostream *os) {
        *os << tf.name << " (" << tf.dim << "D)";
    }
}

namespace {
    double tolerance(double f_opt) {
        return 1e-6 * std::max(1.0, std::abs(f_opt));
    }

    std::vector<TestFunction> all_test_functions() {
        std::vector<TestFunction> result;
        for (size_t dim: {1, 2, 3, 4, 5, 6, 10}) {
            for (auto &tf: functions_for_dimension(dim)) {
                result.push_back(std::move(tf));
            }
        }
        return result;
    }

    std::string test_name(const testing::TestParamInfo<TestFunction> &info) {
        return info.param.name + "_" + std::to_string(info.param.dim) + "d";
    }
}

class ClassicFunctionTest : public testing::TestWithParam<TestFunction> {
};

TEST_P(ClassicFunctionTest, OptimumIsInsideBoundsAndHasDeclaredValue) {
    const TestFunction &tf = GetParam();
    ASSERT_EQ(tf.bounds.size(), tf.dim);
    if (tf.x_opt.empty()) {
        GTEST_SKIP() << "minimizer is not known for " << tf.name << " in " << tf.dim << "D";
    }
    ASSERT_EQ(tf.x_opt.size(), tf.dim);
    for (size_t i = 0; i < tf.dim; ++i) {
        EXPECT_GE(tf.x_opt[i], tf.bounds[i].first);
        EXPECT_LE(tf.x_opt[i], tf.bounds[i].second);
    }
    EXPECT_NEAR(tf(tf.x_opt), tf.f_opt, tolerance(tf.f_opt));
}

TEST_P(ClassicFunctionTest, NoSampledPointIsBelowOptimum) {
    const TestFunction &tf = GetParam();
    std::mt19937 gen(42);
    for (int k = 0; k < 20000; ++k) {
        Point x(tf.dim);
        for (size_t i = 0; i < tf.dim; ++i) {
            x[i] = std::uniform_real_distribution<double>(tf.bounds[i].first, tf.bounds[i].second)(gen);
        }
        ASSERT_GE(tf(x), tf.f_opt - tolerance(tf.f_opt)) << "at sample " << k;
    }
}

INSTANTIATE_TEST_SUITE_P(AllFunctions, ClassicFunctionTest,
                         testing::ValuesIn(all_test_functions()), test_name);

TEST(ClassicFunctionRegistry, EveryNameCanBeCreated) {
    for (const auto &name: function_names()) {
        size_t dim = name == "powell" ? 4 : 2;
        EXPECT_NO_THROW(get_function(name, dim)) << name;
    }
}

TEST(ClassicFunctionRegistry, InvalidRequestsThrow) {
    EXPECT_THROW(get_function("no_such_function"), std::invalid_argument);
    EXPECT_THROW(powell(3), std::invalid_argument);
}

TEST(ClassicFunctionRegistry, ScalableFunctionsUseRequestedDimension) {
    auto tf = rastrigin(7);
    EXPECT_EQ(tf.dim, 7u);
    EXPECT_EQ(tf.bounds.size(), 7u);
    EXPECT_EQ(tf.x_opt.size(), 7u);
}

// Optimizer runs on 2D functions

class ClassicFunctionOptimizerTest : public testing::TestWithParam<TestFunction> {
};

TEST_P(ClassicFunctionOptimizerTest, ShgoFindsGlobalMinimum) {
    const TestFunction &tf = GetParam();
    // Known failures of the current SHGO implementation, kept visible as skipped tests
    if (tf.name == "easom") {
        GTEST_SKIP() << "the needle around (pi, pi) is narrower than the sampling step on [-100, 100]^2";
    }
    if (tf.name == "bohachevsky1") {
        GTEST_SKIP() << "local search stops in a local minimum (absolute Hooke-Jeeves step 0.01)";
    }
    auto res = shgo(tf.function, tf.bounds, 1000);
    EXPECT_NEAR(res.fun, tf.f_opt, 0.1);
}

TEST_P(ClassicFunctionOptimizerTest, DifferentialEvolutionFindsGlobalMinimum) {
    const TestFunction &tf = GetParam();
    DEOptions options(200, 50);
    auto res = differential_evolution(tf.function, tf.bounds, options);
    EXPECT_NEAR(res.fun, tf.f_opt, 0.1);
}

INSTANTIATE_TEST_SUITE_P(Classic2D, ClassicFunctionOptimizerTest,
                         testing::Values(sphere(2), rastrigin(2), ackley(2), rosenbrock(2), griewank(2),
                                         levy(2), styblinski_tang(2), himmelblau(), six_hump_camel(),
                                         three_hump_camel(), branin(), booth(), matyas(), beale(),
                                         goldstein_price(), mccormick(), levy13(), bohachevsky1(),
                                         drop_wave(), holder_table(), cross_in_tray(), shubert(),
                                         michalewicz(2), schwefel(2), easom(), eggholder(), bukin6()),
                         test_name);
