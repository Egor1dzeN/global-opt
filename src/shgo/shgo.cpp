//
// Created by egorm on 3/29/2026.
//
#include <random>
#include "shgo/shgo.h"


std::size_t hash(const Point &point) {
    std::size_t seed = 0;
    for (double val: point) {
        seed ^= std::hash<double>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

[[maybe_unused]] Point
gradient_descent(std::function<double(Point)> func, const Point &start_point, double learning_rate = 0.01,
                 int max_iterations = 1000,
                 double tolerance = 1e-6) {
    Point current_point = start_point;
    double current_value = func(current_point);

    for (int iter = 0; iter < max_iterations; ++iter) {
        Point gradient(current_point.size(), 0.0);
        double eps = 1e-8;

        for (size_t i = 0; i < current_point.size(); ++i) {
            Point point_plus = current_point;
            point_plus[i] += eps;

            Point point_minus = current_point;
            point_minus[i] -= eps;

            gradient[i] = (func(point_plus) - func(point_minus)) / (2 * eps);
        }

        Point new_point = current_point;
        for (size_t i = 0; i < current_point.size(); ++i) {
            new_point[i] -= learning_rate * gradient[i];
        }

        double new_value = func(new_point);

        if (std::abs(new_value - current_value) < tolerance) {
            break;
        }

        if (new_value > current_value) {
            learning_rate *= 0.5;
        } else {
            current_point = new_point;
            current_value = new_value;
        }
    }

    return current_point;
}

static constexpr int HALTON_PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

double halton_element(int index, int base) {
    double result = 0.0;
    double f = 1.0 / base;
    int i = index + 1;
    while (i > 0) {
        result += f * (i % base);
        i /= base;
        f /= base;
    }
    return result;
}

std::vector<Point> generate_points(const std::vector<pdd> &bounds, int n) {
    size_t dim = bounds.size();
    std::vector<Point> points;
    points.reserve(n);

    if (dim <= 12) {
        for (int i = 0; i < n; ++i) {
            Point point(dim);
            for (size_t d = 0; d < dim; ++d) {
                double t = halton_element(i, HALTON_PRIMES[d]);
                point[d] = bounds[d].first + t * (bounds[d].second - bounds[d].first);
            }
            points.push_back(point);
        }
    } else {
        std::mt19937 gen(42);
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        for (int i = 0; i < n; ++i) {
            Point point(dim);
            for (size_t d = 0; d < dim; ++d) {
                double t = dis(gen);
                point[d] = bounds[d].first + t * (bounds[d].second - bounds[d].first);
            }
            points.push_back(point);
        }
    }

    return points;
}

OptimizeResult
shgo(const std::function<double(const std::vector<double> &)> &function,
     const std::vector<std::pair<double, double>> &bounds, int count_point) {
    int call_count = 0;
    Delaunay delaunay;
    delaunay.generate_super_simplex(bounds);
    std::unordered_map<size_t, double> cache;

    auto cached_func = [&function, &call_count, &cache](const Point &p) {
        size_t key = hash(p);
        if (cache.count(key)) return cache[key];
        call_count++;
        return cache[key] = function(p);
    };
    std::vector<Point> points_ = generate_points(bounds, count_point);
    for (auto &point: points_) {
        delaunay.point_insert(point);
    }

    delaunay.delete_super_simplex();


    auto simplexes = delaunay.get_simplexes();
    auto points = delaunay.get_points();

    std::map<Point, double, PointComparator> point_values;
    std::vector<double> simplex_min_values(simplexes.size());

    for (const Point &point: points) {
        point_values[point] = function(point);
    }

    for (size_t s = 0; s < simplexes.size(); ++s) {
        simplex_min_values[s] = std::numeric_limits<double>::max();
        for (const auto &vertex: simplexes[s].vertices) {
            simplex_min_values[s] = std::min(simplex_min_values[s], point_values[vertex]);
        }
    }

    std::vector<Point> minimum_candidates;

    for (const Point &point: points) {
        double val = point_values[point];
        bool is_minimum = true;

        for (size_t s = 0; s < simplexes.size(); ++s) {
            if (simplexes[s].containsVertex(point)) {
                if (val != simplex_min_values[s]) {
                    is_minimum = false;
                    break;
                }
            }
        }

        if (is_minimum) {
            minimum_candidates.push_back(point);
        }
    }
    HookeJeevesMethod hj_method(cached_func,
                                bounds,
                                0.01,
                                1e-8,
                                2.0,
                                8000);
    Point point_min;
    double point_min_val = std::numeric_limits<double>::max();
    for (const auto &candidate: minimum_candidates) {
        Point local_min = hj_method.optimize(candidate);

        double value = cached_func(local_min);
        if (value < point_min_val) {
            point_min_val = value;
            point_min = local_min;
        }
    }
    OptimizeResult optimizeResult;
    optimizeResult.x = point_min;
    optimizeResult.fun = point_min_val;
    optimizeResult.nfev = call_count;
    return optimizeResult;
}

