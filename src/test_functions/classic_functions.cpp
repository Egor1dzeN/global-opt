//
// Classic benchmark functions for global optimization.
//
#include "test_functions/classic_functions.h"

#include <cmath>
#include <limits>
#include <map>
#include <stdexcept>

namespace test_functions {

    namespace {
        constexpr double PI = 3.14159265358979323846;
        constexpr double E = 2.71828182845904523536;

        Bounds box(size_t dim, double lo, double hi) {
            return Bounds(dim, {lo, hi});
        }

        double sqr(double x) { return x * x; }
    }

    // ------------------------------------------------------------------
    // Functions defined for any dimension
    // ------------------------------------------------------------------

    TestFunction sphere(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (double xi: x) s += xi * xi;
            return s;
        };
        return {"sphere", dim, box(dim, -5.12, 5.12), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction rastrigin(size_t dim) {
        auto f = [](const Point &x) {
            double s = 10.0 * x.size();
            for (double xi: x) s += xi * xi - 10.0 * std::cos(2 * PI * xi);
            return s;
        };
        return {"rastrigin", dim, box(dim, -5.12, 5.12), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction ackley(size_t dim) {
        auto f = [](const Point &x) {
            double sum_sq = 0, sum_cos = 0;
            for (double xi: x) {
                sum_sq += xi * xi;
                sum_cos += std::cos(2 * PI * xi);
            }
            double n = static_cast<double>(x.size());
            return -20.0 * std::exp(-0.2 * std::sqrt(sum_sq / n)) - std::exp(sum_cos / n) + 20.0 + E;
        };
        return {"ackley", dim, box(dim, -32.768, 32.768), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction rosenbrock(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i + 1 < x.size(); ++i) {
                s += 100.0 * sqr(x[i + 1] - x[i] * x[i]) + sqr(x[i] - 1.0);
            }
            return s;
        };
        return {"rosenbrock", dim, box(dim, -5.0, 10.0), f, 0.0, Point(dim, 1.0)};
    }

    TestFunction griewank(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0, p = 1;
            for (size_t i = 0; i < x.size(); ++i) {
                s += x[i] * x[i] / 4000.0;
                p *= std::cos(x[i] / std::sqrt(static_cast<double>(i + 1)));
            }
            return s - p + 1.0;
        };
        return {"griewank", dim, box(dim, -600.0, 600.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction schwefel(size_t dim) {
        auto f = [](const Point &x) {
            double s = 418.9828872724338 * x.size();
            for (double xi: x) s -= xi * std::sin(std::sqrt(std::abs(xi)));
            return s;
        };
        return {"schwefel", dim, box(dim, -500.0, 500.0), f, 0.0, Point(dim, 420.9687463599820)};
    }

    TestFunction levy(size_t dim) {
        auto f = [](const Point &x) {
            size_t n = x.size();
            auto w = [&x](size_t i) { return 1.0 + (x[i] - 1.0) / 4.0; };
            double s = sqr(std::sin(PI * w(0)));
            for (size_t i = 0; i + 1 < n; ++i) {
                s += sqr(w(i) - 1.0) * (1.0 + 10.0 * sqr(std::sin(PI * w(i) + 1.0)));
            }
            s += sqr(w(n - 1) - 1.0) * (1.0 + sqr(std::sin(2 * PI * w(n - 1))));
            return s;
        };
        return {"levy", dim, box(dim, -10.0, 10.0), f, 0.0, Point(dim, 1.0)};
    }

    TestFunction zakharov(size_t dim) {
        auto f = [](const Point &x) {
            double s1 = 0, s2 = 0;
            for (size_t i = 0; i < x.size(); ++i) {
                s1 += x[i] * x[i];
                s2 += 0.5 * static_cast<double>(i + 1) * x[i];
            }
            return s1 + s2 * s2 + s2 * s2 * s2 * s2;
        };
        return {"zakharov", dim, box(dim, -5.0, 10.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction styblinski_tang(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (double xi: x) s += xi * xi * xi * xi - 16.0 * xi * xi + 5.0 * xi;
            return 0.5 * s;
        };
        return {"styblinski_tang", dim, box(dim, -5.0, 5.0), f,
                -39.16616570377142 * static_cast<double>(dim), Point(dim, -2.903534027771178)};
    }

    TestFunction dixon_price(size_t dim) {
        auto f = [](const Point &x) {
            double s = sqr(x[0] - 1.0);
            for (size_t i = 1; i < x.size(); ++i) {
                s += static_cast<double>(i + 1) * sqr(2.0 * x[i] * x[i] - x[i - 1]);
            }
            return s;
        };
        Point x_opt(dim);
        for (size_t i = 0; i < dim; ++i) {
            double p = std::pow(2.0, static_cast<double>(i + 1));
            x_opt[i] = std::pow(2.0, -(p - 2.0) / p);
        }
        return {"dixon_price", dim, box(dim, -10.0, 10.0), f, 0.0, x_opt};
    }

    TestFunction sum_squares(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i < x.size(); ++i) s += static_cast<double>(i + 1) * x[i] * x[i];
            return s;
        };
        return {"sum_squares", dim, box(dim, -10.0, 10.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction rotated_hyper_ellipsoid(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0, prefix = 0;
            for (double xi: x) {
                prefix += xi * xi;
                s += prefix;
            }
            return s;
        };
        return {"rotated_hyper_ellipsoid", dim, box(dim, -65.536, 65.536), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction sum_of_different_powers(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i < x.size(); ++i) s += std::pow(std::abs(x[i]), static_cast<double>(i + 2));
            return s;
        };
        return {"sum_of_different_powers", dim, box(dim, -1.0, 1.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction trid(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i < x.size(); ++i) {
                s += sqr(x[i] - 1.0);
                if (i > 0) s -= x[i] * x[i - 1];
            }
            return s;
        };
        double d = static_cast<double>(dim);
        Point x_opt(dim);
        for (size_t i = 0; i < dim; ++i) {
            double k = static_cast<double>(i + 1);
            x_opt[i] = k * (d + 1.0 - k);
        }
        return {"trid", dim, box(dim, -d * d, d * d), f, -d * (d + 4.0) * (d - 1.0) / 6.0, x_opt};
    }

    TestFunction powell(size_t dim) {
        if (dim % 4 != 0) {
            throw std::invalid_argument("powell: dimension must be a multiple of 4");
        }
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i + 3 < x.size(); i += 4) {
                s += sqr(x[i] + 10.0 * x[i + 1])
                     + 5.0 * sqr(x[i + 2] - x[i + 3])
                     + std::pow(x[i + 1] - 2.0 * x[i + 2], 4)
                     + 10.0 * std::pow(x[i] - x[i + 3], 4);
            }
            return s;
        };
        return {"powell", dim, box(dim, -4.0, 5.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction alpine1(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (double xi: x) s += std::abs(xi * std::sin(xi) + 0.1 * xi);
            return s;
        };
        return {"alpine1", dim, box(dim, -10.0, 10.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction salomon(size_t dim) {
        auto f = [](const Point &x) {
            double r = 0;
            for (double xi: x) r += xi * xi;
            r = std::sqrt(r);
            return 1.0 - std::cos(2 * PI * r) + 0.1 * r;
        };
        return {"salomon", dim, box(dim, -100.0, 100.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction qing(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i < x.size(); ++i) s += sqr(x[i] * x[i] - static_cast<double>(i + 1));
            return s;
        };
        Point x_opt(dim);
        for (size_t i = 0; i < dim; ++i) x_opt[i] = std::sqrt(static_cast<double>(i + 1));
        return {"qing", dim, box(dim, -500.0, 500.0), f, 0.0, x_opt};
    }

    TestFunction exponential(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (double xi: x) s += xi * xi;
            return -std::exp(-0.5 * s);
        };
        return {"exponential", dim, box(dim, -1.0, 1.0), f, -1.0, Point(dim, 0.0)};
    }

    TestFunction bent_cigar(size_t dim) {
        auto f = [](const Point &x) {
            double s = x[0] * x[0];
            for (size_t i = 1; i < x.size(); ++i) s += 1e6 * x[i] * x[i];
            return s;
        };
        return {"bent_cigar", dim, box(dim, -100.0, 100.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction discus(size_t dim) {
        auto f = [](const Point &x) {
            double s = 1e6 * x[0] * x[0];
            for (size_t i = 1; i < x.size(); ++i) s += x[i] * x[i];
            return s;
        };
        return {"discus", dim, box(dim, -100.0, 100.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction schwefel_2_22(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0, p = 1;
            for (double xi: x) {
                s += std::abs(xi);
                p *= std::abs(xi);
            }
            return s + p;
        };
        return {"schwefel_2_22", dim, box(dim, -10.0, 10.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction step(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (double xi: x) s += sqr(std::floor(xi + 0.5));
            return s;
        };
        return {"step", dim, box(dim, -100.0, 100.0), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction quartic(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i < x.size(); ++i) s += static_cast<double>(i + 1) * std::pow(x[i], 4);
            return s;
        };
        return {"quartic", dim, box(dim, -1.28, 1.28), f, 0.0, Point(dim, 0.0)};
    }

    TestFunction michalewicz(size_t dim) {
        auto f = [](const Point &x) {
            double s = 0;
            for (size_t i = 0; i < x.size(); ++i) {
                s -= std::sin(x[i]) * std::pow(std::sin(static_cast<double>(i + 1) * x[i] * x[i] / PI), 20);
            }
            return s;
        };
        double f_opt = std::numeric_limits<double>::quiet_NaN();
        Point x_opt;
        if (dim == 2) {
            f_opt = -1.8013034100985537;
            x_opt = {2.202905513296628, 1.570796326794897};
        } else if (dim == 5) {
            f_opt = -4.687658179;
        } else if (dim == 10) {
            f_opt = -9.66015;
        }
        return {"michalewicz", dim, box(dim, 0.0, PI), f, f_opt, x_opt};
    }

    // ------------------------------------------------------------------
    // Functions of fixed dimension
    // ------------------------------------------------------------------

    TestFunction beale() {
        auto f = [](const Point &x) {
            double a = x[0], b = x[1];
            return sqr(1.5 - a + a * b) + sqr(2.25 - a + a * b * b) + sqr(2.625 - a + a * b * b * b);
        };
        return {"beale", 2, box(2, -4.5, 4.5), f, 0.0, {3.0, 0.5}};
    }

    TestFunction booth() {
        auto f = [](const Point &x) {
            return sqr(x[0] + 2 * x[1] - 7) + sqr(2 * x[0] + x[1] - 5);
        };
        return {"booth", 2, box(2, -10.0, 10.0), f, 0.0, {1.0, 3.0}};
    }

    TestFunction matyas() {
        auto f = [](const Point &x) {
            return 0.26 * (x[0] * x[0] + x[1] * x[1]) - 0.48 * x[0] * x[1];
        };
        return {"matyas", 2, box(2, -10.0, 10.0), f, 0.0, {0.0, 0.0}};
    }

    TestFunction himmelblau() {
        auto f = [](const Point &x) {
            return sqr(x[0] * x[0] + x[1] - 11) + sqr(x[0] + x[1] * x[1] - 7);
        };
        return {"himmelblau", 2, box(2, -5.0, 5.0), f, 0.0, {3.0, 2.0}};
    }

    TestFunction three_hump_camel() {
        auto f = [](const Point &x) {
            double a = x[0], b = x[1];
            return 2 * a * a - 1.05 * std::pow(a, 4) + std::pow(a, 6) / 6.0 + a * b + b * b;
        };
        return {"three_hump_camel", 2, box(2, -5.0, 5.0), f, 0.0, {0.0, 0.0}};
    }

    TestFunction six_hump_camel() {
        auto f = [](const Point &x) {
            double a = x[0], b = x[1];
            return (4 - 2.1 * a * a + std::pow(a, 4) / 3.0) * a * a + a * b + (-4 + 4 * b * b) * b * b;
        };
        return {"six_hump_camel", 2, {{-3.0, 3.0}, {-2.0, 2.0}}, f,
                -1.031628453489877, {0.0898420131003, -0.7126564030207}};
    }

    TestFunction easom() {
        auto f = [](const Point &x) {
            return -std::cos(x[0]) * std::cos(x[1]) * std::exp(-(sqr(x[0] - PI) + sqr(x[1] - PI)));
        };
        return {"easom", 2, box(2, -100.0, 100.0), f, -1.0, {PI, PI}};
    }

    TestFunction goldstein_price() {
        auto f = [](const Point &x) {
            double a = x[0], b = x[1];
            double t1 = 1 + sqr(a + b + 1) * (19 - 14 * a + 3 * a * a - 14 * b + 6 * a * b + 3 * b * b);
            double t2 = 30 + sqr(2 * a - 3 * b) * (18 - 32 * a + 12 * a * a + 48 * b - 36 * a * b + 27 * b * b);
            return t1 * t2;
        };
        return {"goldstein_price", 2, box(2, -2.0, 2.0), f, 3.0, {0.0, -1.0}};
    }

    TestFunction branin() {
        auto f = [](const Point &x) {
            const double b = 5.1 / (4 * PI * PI), c = 5.0 / PI, t = 1.0 / (8 * PI);
            return sqr(x[1] - b * x[0] * x[0] + c * x[0] - 6.0) + 10.0 * (1 - t) * std::cos(x[0]) + 10.0;
        };
        return {"branin", 2, {{-5.0, 10.0}, {0.0, 15.0}}, f, 0.397887357729739, {PI, 2.275}};
    }

    TestFunction bukin6() {
        auto f = [](const Point &x) {
            return 100.0 * std::sqrt(std::abs(x[1] - 0.01 * x[0] * x[0])) + 0.01 * std::abs(x[0] + 10.0);
        };
        return {"bukin6", 2, {{-15.0, -5.0}, {-3.0, 3.0}}, f, 0.0, {-10.0, 1.0}};
    }

    TestFunction cross_in_tray() {
        auto f = [](const Point &x) {
            double r = std::sqrt(x[0] * x[0] + x[1] * x[1]);
            double v = std::abs(std::sin(x[0]) * std::sin(x[1]) * std::exp(std::abs(100.0 - r / PI)));
            return -0.0001 * std::pow(v + 1.0, 0.1);
        };
        return {"cross_in_tray", 2, box(2, -10.0, 10.0), f,
                -2.062611870822739, {1.349406608602084, 1.349406608602084}};
    }

    TestFunction drop_wave() {
        auto f = [](const Point &x) {
            double r2 = x[0] * x[0] + x[1] * x[1];
            return -(1.0 + std::cos(12.0 * std::sqrt(r2))) / (0.5 * r2 + 2.0);
        };
        return {"drop_wave", 2, box(2, -5.12, 5.12), f, -1.0, {0.0, 0.0}};
    }

    TestFunction eggholder() {
        auto f = [](const Point &x) {
            double a = x[0], b = x[1] + 47.0;
            return -b * std::sin(std::sqrt(std::abs(b + a / 2.0))) - a * std::sin(std::sqrt(std::abs(a - b)));
        };
        return {"eggholder", 2, box(2, -512.0, 512.0), f, -959.6406627208506, {512.0, 404.2318058008512}};
    }

    TestFunction holder_table() {
        auto f = [](const Point &x) {
            double r = std::sqrt(x[0] * x[0] + x[1] * x[1]);
            return -std::abs(std::sin(x[0]) * std::cos(x[1]) * std::exp(std::abs(1.0 - r / PI)));
        };
        return {"holder_table", 2, box(2, -10.0, 10.0), f,
                -19.20850256788675, {8.05502347573655, 9.66459041730769}};
    }

    TestFunction levy13() {
        auto f = [](const Point &x) {
            double a = x[0], b = x[1];
            return sqr(std::sin(3 * PI * a))
                   + sqr(a - 1) * (1 + sqr(std::sin(3 * PI * b)))
                   + sqr(b - 1) * (1 + sqr(std::sin(2 * PI * b)));
        };
        return {"levy13", 2, box(2, -10.0, 10.0), f, 0.0, {1.0, 1.0}};
    }

    TestFunction schaffer2() {
        auto f = [](const Point &x) {
            double a2 = x[0] * x[0], b2 = x[1] * x[1];
            return 0.5 + (sqr(std::sin(a2 - b2)) - 0.5) / sqr(1.0 + 0.001 * (a2 + b2));
        };
        return {"schaffer2", 2, box(2, -100.0, 100.0), f, 0.0, {0.0, 0.0}};
    }

    TestFunction schaffer4() {
        auto f = [](const Point &x) {
            double a2 = x[0] * x[0], b2 = x[1] * x[1];
            return 0.5 + (sqr(std::cos(std::sin(std::abs(a2 - b2)))) - 0.5) / sqr(1.0 + 0.001 * (a2 + b2));
        };
        return {"schaffer4", 2, box(2, -100.0, 100.0), f, 0.292578632035980, {0.0, 1.253131828792882}};
    }

    TestFunction mccormick() {
        auto f = [](const Point &x) {
            return std::sin(x[0] + x[1]) + sqr(x[0] - x[1]) - 1.5 * x[0] + 2.5 * x[1] + 1.0;
        };
        return {"mccormick", 2, {{-1.5, 4.0}, {-3.0, 4.0}}, f,
                -1.913222954981037, {-0.547197551196598, -1.547197551196598}};
    }

    TestFunction shubert() {
        auto f = [](const Point &x) {
            double s1 = 0, s2 = 0;
            for (int i = 1; i <= 5; ++i) {
                s1 += i * std::cos((i + 1) * x[0] + i);
                s2 += i * std::cos((i + 1) * x[1] + i);
            }
            return s1 * s2;
        };
        return {"shubert", 2, box(2, -10.0, 10.0), f, -186.7309088310239, {-7.083506406692, -7.708313735499}};
    }

    TestFunction bohachevsky1() {
        auto f = [](const Point &x) {
            return x[0] * x[0] + 2 * x[1] * x[1] - 0.3 * std::cos(3 * PI * x[0]) - 0.4 * std::cos(4 * PI * x[1]) + 0.7;
        };
        return {"bohachevsky1", 2, box(2, -100.0, 100.0), f, 0.0, {0.0, 0.0}};
    }

    TestFunction leon() {
        auto f = [](const Point &x) {
            return 100.0 * sqr(x[1] - x[0] * x[0] * x[0]) + sqr(1.0 - x[0]);
        };
        return {"leon", 2, box(2, -1.2, 1.2), f, 0.0, {1.0, 1.0}};
    }

    TestFunction bird() {
        auto f = [](const Point &x) {
            double a = x[0], b = x[1];
            return std::sin(a) * std::exp(sqr(1 - std::cos(b)))
                   + std::cos(b) * std::exp(sqr(1 - std::sin(a)))
                   + sqr(a - b);
        };
        return {"bird", 2, box(2, -2 * PI, 2 * PI), f, -106.7645367492648, {4.701043133313481, 3.152938506076334}};
    }

    TestFunction de_jong5() {
        auto f = [](const Point &x) {
            static const double grid[5] = {-32.0, -16.0, 0.0, 16.0, 32.0};
            double s = 0.002;
            for (int j = 0; j < 25; ++j) {
                double a1 = grid[j % 5], a2 = grid[j / 5];
                s += 1.0 / ((j + 1) + std::pow(x[0] - a1, 6) + std::pow(x[1] - a2, 6));
            }
            return 1.0 / s;
        };
        return {"de_jong5", 2, box(2, -65.536, 65.536), f, 0.9980038377944496, {-31.9783347961732, -31.97833344709552}};
    }

    namespace {
        double hartmann(const Point &x, const double alpha[4], const double *A, const double *P, size_t n) {
            double s = 0;
            for (size_t i = 0; i < 4; ++i) {
                double inner = 0;
                for (size_t j = 0; j < n; ++j) inner += A[i * n + j] * sqr(x[j] - P[i * n + j]);
                s -= alpha[i] * std::exp(-inner);
            }
            return s;
        }

        const double HARTMANN_ALPHA[4] = {1.0, 1.2, 3.0, 3.2};
    }

    TestFunction hartmann3() {
        auto f = [](const Point &x) {
            static const double A[12] = {3.0, 10, 30,
                                         0.1, 10, 35,
                                         3.0, 10, 30,
                                         0.1, 10, 35};
            static const double P[12] = {0.3689, 0.1170, 0.2673,
                                         0.4699, 0.4387, 0.7470,
                                         0.1091, 0.8732, 0.5547,
                                         0.0381, 0.5743, 0.8828};
            return hartmann(x, HARTMANN_ALPHA, A, P, 3);
        };
        return {"hartmann3", 3, box(3, 0.0, 1.0), f, -3.862779787332663,
                {0.11458886687077, 0.5556488952005589, 0.8525469851200751}};
    }

    TestFunction hartmann6() {
        auto f = [](const Point &x) {
            static const double A[24] = {10, 3, 17, 3.5, 1.7, 8,
                                         0.05, 10, 17, 0.1, 8, 14,
                                         3, 3.5, 1.7, 10, 17, 8,
                                         17, 8, 0.05, 10, 0.1, 14};
            static const double P[24] = {0.1312, 0.1696, 0.5569, 0.0124, 0.8283, 0.5886,
                                         0.2329, 0.4135, 0.8307, 0.3736, 0.1004, 0.9991,
                                         0.2348, 0.1451, 0.3522, 0.2883, 0.3047, 0.6650,
                                         0.4047, 0.8828, 0.8732, 0.5743, 0.1091, 0.0381};
            return hartmann(x, HARTMANN_ALPHA, A, P, 6);
        };
        return {"hartmann6", 6, box(6, 0.0, 1.0), f, -3.32236801141551,
                {0.20169, 0.150011, 0.476874, 0.275332, 0.311652, 0.6573}};
    }

    TestFunction colville() {
        auto f = [](const Point &x) {
            return 100.0 * sqr(x[0] * x[0] - x[1]) + sqr(x[0] - 1.0) + sqr(x[2] - 1.0)
                   + 90.0 * sqr(x[2] * x[2] - x[3])
                   + 10.1 * (sqr(x[1] - 1.0) + sqr(x[3] - 1.0))
                   + 19.8 * (x[1] - 1.0) * (x[3] - 1.0);
        };
        return {"colville", 4, box(4, -10.0, 10.0), f, 0.0, {1.0, 1.0, 1.0, 1.0}};
    }

    namespace {
        TestFunction shekel(int m, double f_opt, const Point &x_opt) {
            auto f = [m](const Point &x) {
                static const double beta[10] = {1, 2, 2, 4, 4, 6, 3, 7, 5, 5};
                static const double C[4][10] = {{4, 1, 8, 6, 3, 2, 5, 8, 6, 7},
                                                {4, 1, 8, 6, 7, 9, 3, 1, 2, 3.6},
                                                {4, 1, 8, 6, 3, 2, 5, 8, 6, 7},
                                                {4, 1, 8, 6, 7, 9, 3, 1, 2, 3.6}};
                double s = 0;
                for (int i = 0; i < m; ++i) {
                    double inner = 0.1 * beta[i];
                    for (int j = 0; j < 4; ++j) inner += sqr(x[j] - C[j][i]);
                    s -= 1.0 / inner;
                }
                return s;
            };
            return {"shekel" + std::to_string(m), 4, box(4, 0.0, 10.0), f, f_opt, x_opt};
        }
    }

    // Optima of shekel7, shekel10 and hartmann3 are refined numerically for exactly these
    // coefficients; the often quoted -10.4029, -10.5364 and -3.86278 come from variants
    // with slightly different constants and lie below the true minimum of this definition.
    TestFunction shekel5() {
        return shekel(5, -10.1531996790582, {4.00003715, 4.00013327, 4.00003715, 4.00013327});
    }

    TestFunction shekel7() {
        return shekel(7, -10.40291533677775, {4.000572823532698, 3.999606209987876, 4.000572820791202, 3.999606206616869});
    }

    TestFunction shekel10() {
        return shekel(10, -10.53644315348353, {4.000746867116609, 3.999509478856615, 4.000746866141904, 3.99950947792584});
    }

    // ------------------------------------------------------------------
    // Registry
    // ------------------------------------------------------------------

    namespace {
        using ScalableFactory = TestFunction (*)(size_t);
        using FixedFactory = TestFunction (*)();

        const std::map<std::string, ScalableFactory> &scalable_registry() {
            static const std::map<std::string, ScalableFactory> registry = {
                    {"sphere",                  sphere},
                    {"rastrigin",               rastrigin},
                    {"ackley",                  ackley},
                    {"rosenbrock",              rosenbrock},
                    {"griewank",                griewank},
                    {"schwefel",                schwefel},
                    {"levy",                    levy},
                    {"zakharov",                zakharov},
                    {"styblinski_tang",         styblinski_tang},
                    {"dixon_price",             dixon_price},
                    {"sum_squares",             sum_squares},
                    {"rotated_hyper_ellipsoid", rotated_hyper_ellipsoid},
                    {"sum_of_different_powers", sum_of_different_powers},
                    {"trid",                    trid},
                    {"powell",                  powell},
                    {"alpine1",                 alpine1},
                    {"salomon",                 salomon},
                    {"qing",                    qing},
                    {"exponential",             exponential},
                    {"bent_cigar",              bent_cigar},
                    {"discus",                  discus},
                    {"schwefel_2_22",           schwefel_2_22},
                    {"step",                    step},
                    {"quartic",                 quartic},
                    {"michalewicz",             michalewicz},
            };
            return registry;
        }

        const std::map<std::string, FixedFactory> &fixed_registry() {
            static const std::map<std::string, FixedFactory> registry = {
                    {"beale",            beale},
                    {"booth",            booth},
                    {"matyas",           matyas},
                    {"himmelblau",       himmelblau},
                    {"three_hump_camel", three_hump_camel},
                    {"six_hump_camel",   six_hump_camel},
                    {"easom",            easom},
                    {"goldstein_price",  goldstein_price},
                    {"branin",           branin},
                    {"bukin6",           bukin6},
                    {"cross_in_tray",    cross_in_tray},
                    {"drop_wave",        drop_wave},
                    {"eggholder",        eggholder},
                    {"holder_table",     holder_table},
                    {"levy13",           levy13},
                    {"schaffer2",        schaffer2},
                    {"schaffer4",        schaffer4},
                    {"mccormick",        mccormick},
                    {"shubert",          shubert},
                    {"bohachevsky1",     bohachevsky1},
                    {"leon",             leon},
                    {"bird",             bird},
                    {"de_jong5",         de_jong5},
                    {"hartmann3",        hartmann3},
                    {"hartmann6",        hartmann6},
                    {"colville",         colville},
                    {"shekel5",          shekel5},
                    {"shekel7",          shekel7},
                    {"shekel10",         shekel10},
            };
            return registry;
        }

        bool is_defined_for(const std::string &name, size_t dim) {
            if (name == "powell") return dim % 4 == 0;
            if (name == "rosenbrock" || name == "bent_cigar") return dim >= 2;
            if (name == "michalewicz") return dim == 2 || dim == 5 || dim == 10;
            return dim >= 1;
        }
    }

    std::vector<TestFunction> functions_for_dimension(size_t dim) {
        std::vector<TestFunction> result;
        for (const auto &[name, factory]: scalable_registry()) {
            if (is_defined_for(name, dim)) result.push_back(factory(dim));
        }
        for (const auto &[name, factory]: fixed_registry()) {
            TestFunction tf = factory();
            if (tf.dim == dim) result.push_back(std::move(tf));
        }
        return result;
    }

    std::vector<TestFunction> fixed_dimension_functions() {
        std::vector<TestFunction> result;
        for (const auto &[name, factory]: fixed_registry()) result.push_back(factory());
        return result;
    }

    TestFunction get_function(const std::string &name, size_t dim) {
        if (auto it = scalable_registry().find(name); it != scalable_registry().end()) {
            return it->second(dim);
        }
        if (auto it = fixed_registry().find(name); it != fixed_registry().end()) {
            return it->second();
        }
        throw std::invalid_argument("Unknown test function: " + name);
    }

    std::vector<std::string> function_names() {
        std::vector<std::string> names;
        for (const auto &[name, factory]: scalable_registry()) names.push_back(name);
        for (const auto &[name, factory]: fixed_registry()) names.push_back(name);
        return names;
    }
}
