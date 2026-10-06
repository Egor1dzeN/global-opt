//
// Classic benchmark functions for global optimization.
// Formulas, bounds and optima follow the Virtual Library of Simulation Experiments
// (S. Surjanovic, D. Bingham) and Jamil & Yang, "A literature survey of benchmark
// functions for global optimization problems" (2013).
//

#ifndef FIND_GLOBAL_OPT_CLASSIC_FUNCTIONS_H
#define FIND_GLOBAL_OPT_CLASSIC_FUNCTIONS_H

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace test_functions {

    using Point = std::vector<double>;
    using Bounds = std::vector<std::pair<double, double>>;

    struct TestFunction {
        std::string name;
        size_t dim;
        Bounds bounds;
        std::function<double(const Point &)> function;
        double f_opt;   // global minimum value
        Point x_opt;    // one of the global minimizers

        double operator()(const Point &x) const { return function(x); }
    };

    // Functions defined for any dimension (dim >= 1 unless stated otherwise)
    TestFunction sphere(size_t dim);
    TestFunction rastrigin(size_t dim);
    TestFunction ackley(size_t dim);
    TestFunction rosenbrock(size_t dim);              // dim >= 2
    TestFunction griewank(size_t dim);
    TestFunction schwefel(size_t dim);
    TestFunction levy(size_t dim);
    TestFunction zakharov(size_t dim);
    TestFunction styblinski_tang(size_t dim);
    TestFunction dixon_price(size_t dim);
    TestFunction sum_squares(size_t dim);
    TestFunction rotated_hyper_ellipsoid(size_t dim);
    TestFunction sum_of_different_powers(size_t dim);
    TestFunction trid(size_t dim);
    TestFunction powell(size_t dim);                  // dim is a multiple of 4
    TestFunction alpine1(size_t dim);
    TestFunction salomon(size_t dim);
    TestFunction qing(size_t dim);
    TestFunction exponential(size_t dim);
    TestFunction bent_cigar(size_t dim);              // dim >= 2
    TestFunction discus(size_t dim);
    TestFunction schwefel_2_22(size_t dim);
    TestFunction step(size_t dim);
    TestFunction quartic(size_t dim);
    TestFunction michalewicz(size_t dim);             // optimum known for dim = 2, 5, 10

    // Functions of fixed dimension
    TestFunction beale();                // 2D
    TestFunction booth();                // 2D
    TestFunction matyas();               // 2D
    TestFunction himmelblau();           // 2D, four global minima
    TestFunction three_hump_camel();     // 2D
    TestFunction six_hump_camel();       // 2D, two global minima
    TestFunction easom();                // 2D, needle in a flat landscape
    TestFunction goldstein_price();      // 2D
    TestFunction branin();               // 2D, three global minima
    TestFunction bukin6();               // 2D, narrow curved valley
    TestFunction cross_in_tray();        // 2D, four global minima
    TestFunction drop_wave();            // 2D
    TestFunction eggholder();            // 2D, minimum on the boundary
    TestFunction holder_table();         // 2D, four global minima
    TestFunction levy13();               // 2D
    TestFunction schaffer2();            // 2D
    TestFunction schaffer4();            // 2D
    TestFunction mccormick();            // 2D
    TestFunction shubert();              // 2D, 18 global minima
    TestFunction bohachevsky1();         // 2D
    TestFunction leon();                 // 2D
    TestFunction bird();                 // 2D
    TestFunction de_jong5();             // 2D, Shekel's foxholes
    TestFunction hartmann3();            // 3D
    TestFunction hartmann6();            // 6D
    TestFunction colville();             // 4D
    TestFunction shekel5();              // 4D
    TestFunction shekel7();              // 4D
    TestFunction shekel10();             // 4D

    // Every function that is defined for the given dimension: the scalable ones
    // (with a known optimum) plus the fixed-dimension ones whose dimension matches.
    std::vector<TestFunction> functions_for_dimension(size_t dim);

    // All fixed-dimension functions
    std::vector<TestFunction> fixed_dimension_functions();

    // Lookup by name ("rastrigin", "six_hump_camel", ...); dim is ignored
    // for fixed-dimension functions. Throws std::invalid_argument if unknown.
    TestFunction get_function(const std::string &name, size_t dim = 2);

    std::vector<std::string> function_names();
}

#endif //FIND_GLOBAL_OPT_CLASSIC_FUNCTIONS_H
