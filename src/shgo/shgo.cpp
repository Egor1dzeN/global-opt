//
// Created by egorm on 3/29/2026.
//
#include "shgo/shgo.h"


std::size_t hash(const Point &point) {
    std::size_t seed = 0;
    for (double val: point) {
        seed ^= std::hash<double>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

Point gradient_descent(std::function<double(Point)> func, const Point &start_point, double learning_rate = 0.01,
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

std::vector<Point> generate_points(const std::vector<pdd> &bounds, int n) {
    size_t dim = bounds.size();
    int total_points = static_cast<int>(std::pow(n, dim));

    std::vector<Point> points;
    points.reserve(total_points);

    std::vector<int> indices(dim, 0);

    while (true) {
        std::vector<double> coords(dim);
        for (int d = 0; d < dim; ++d) {
            double min_val = bounds[d].first;
            double max_val = bounds[d].second;
            double t = static_cast<double>(indices[d]) / (n - 1);
            coords[d] = min_val + t * (max_val - min_val);
        }
        points.emplace_back(coords);

        int d = int(dim) - 1;
        while (d >= 0 && ++indices[d] >= n) {
            indices[d] = 0;
            --d;
        }

        if (d < 0) {
            break;
        }
    }

    return points;
}

OptimizeResult
shgo(const std::function<double(const std::vector<double> &)> &function,
     const std::vector<std::pair<double, double>> &bounds) {
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
    std::vector<Point> points_ = generate_points(bounds, 300);
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
                                bounds, // передаем границы
                                0.01, // начальный шаг
                                1e-8, // точность
                                2.0, // множитель шага
                                5000); // макс. итераций
    Point point_min;
    double point_min_val = std::numeric_limits<double>::max();
    for (const auto &candidate: minimum_candidates) {
        Point local_min = hj_method.optimize(candidate);
//        Point local_min = local_minimize(function, candidate);

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
