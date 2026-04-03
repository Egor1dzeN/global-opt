//
// Created by egorm on 16-Mar-26.
//

#ifndef SHGO2_DELAUNAY_H
#define SHGO2_DELAUNAY_H

#include "simplex.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include "tools.h"

using Point = std::vector<double>;
const double EPS = 1e-9;

struct PointComparator {
    bool operator()(const Point &a, const Point &b) const {
        for (size_t i = 0; i < a.size(); ++i) {
            if (std::abs(a[i] - b[i]) > EPS) {
                return a[i] < b[i];
            }
        }
        return false;
    }
};

struct FacetComparator {
    bool operator()(const std::vector<Point> &a, const std::vector<Point> &b) const {
        if (a.size() != b.size()) return a.size() < b.size();
        PointComparator pc;
        for (size_t i = 0; i < a.size(); ++i) {
            if (pc(a[i], b[i])) return true;
            if (pc(b[i], a[i])) return false;
        }
        return false;
    }
};


inline std::vector<double> solveLinearSystem(std::vector<std::vector<double>>& A, std::vector<double>& b) {
    size_t n = A.size();

    for (size_t i = 0; i < n; ++i) {
        size_t max_row = i;
        for (size_t k = i + 1; k < n; ++k) {
            if (std::abs(A[k][i]) > std::abs(A[max_row][i])) {
                max_row = k;
            }
        }

        if (std::abs(A[max_row][i]) < 1e-10) {
            throw std::runtime_error("Матрица вырождена - точки лежат на одной сфере меньшей размерности");
        }

        std::swap(A[i], A[max_row]);
        std::swap(b[i], b[max_row]);

        double divisor = A[i][i];
        for (size_t j = i; j < n; ++j) {
            A[i][j] /= divisor;
        }
        b[i] /= divisor;

        for (size_t k = 0; k < n; ++k) {
            if (k != i) {
                double factor = A[k][i];
                for (size_t j = i; j < n; ++j) {
                    A[k][j] -= factor * A[i][j];
                }
                b[k] -= factor * b[i];
            }
        }
    }

    return b;
}

inline std::pair<Point, double> circumsphere(const Simplex &simplex) {
    size_t n = simplex.getDimension();
    std::vector<std::vector<double>> A(n, std::vector<double>(n));
    std::vector<double> b(n);

    const Point &p0 = simplex.vertices[0];

    for (size_t i = 1; i <= n; ++i) {
        const Point &pi = simplex.vertices[i];
        double pi_p0_norm_sq = 0;
        for (size_t j = 0; j < n; ++j) {
            double diff = pi[j] - p0[j];
            A[i - 1][j] = diff; // Вектор pi - p0
            pi_p0_norm_sq += diff * diff;
        }
        b[i - 1] = pi_p0_norm_sq / 2.0;
    }

    std::vector<double> center_rel = solveLinearSystem(A, b);

    Point center(n);
    double radius_sq = 0;
    for (size_t i = 0; i < n; ++i) {
        center[i] = p0[i] + center_rel[i];
        radius_sq += center_rel[i] * center_rel[i];
    }

    return {center, std::sqrt(radius_sq)};
}

class Delaunay {
private:
    std::vector<Point> super_simplex_vertices;
    std::vector<Simplex> simplexes;

    std::vector<Point> getFace(const Simplex &simplex, size_t exclude_index) {
        std::vector<Point> face;
        for (size_t i = 0; i < simplex.vertices.size(); ++i) {
            if (i != exclude_index) {
                face.push_back(simplex.vertices[i]);
            }
        }
        return face;
    }

    double distance(const Point &p1, const Point &p2) {
        double dist_sq = 0;
        for (size_t i = 0; i < p1.size(); ++i) {
            double diff = p1[i] - p2[i];
            dist_sq += diff * diff;
        }
        return dist_sq;
    }

public:
    void generate_super_simplex(const std::vector<std::pair<double, double>> &bounds) {
        size_t dimension = bounds.size();

        Point center(dimension);
        std::vector<double> half_ranges(dimension);

        for (size_t i = 0; i < dimension; ++i) {
            double minVal = std::min(bounds[i].first, bounds[i].second);
            double maxVal = std::max(bounds[i].first, bounds[i].second);
            half_ranges[i] = (maxVal - minVal) / 2.0;
            center[i] = minVal + half_ranges[i];
        }

        double maxHalfRange = *std::max_element(half_ranges.begin(), half_ranges.end());
        double radius = maxHalfRange * 3.0;

        super_simplex_vertices.clear();

        Point v0 = center;
        for (size_t i = 0; i < dimension; ++i) {
            v0[i] -= radius;
        }
        super_simplex_vertices.push_back(v0);

        for (size_t i = 0; i < dimension; ++i) {
            Point vi = center;
            for (size_t j = 0; j < dimension; ++j) {
                if (j == i) {
                    vi[j] += radius * dimension;
                } else {
                    vi[j] -= radius;
                }
            }
            super_simplex_vertices.push_back(vi);
        }

        simplexes.emplace_back(super_simplex_vertices);

//        std::cout << "Super simplex (" << dimension << "D):\n";
//        for (size_t i = 0; i < super_simplex_vertices.size(); ++i) {
//            std::cout << "v" << i << ": ";
//            for (size_t j = 0; j < dimension; ++j) {
//                std::cout << super_simplex_vertices[i][j];
//                if (j < dimension - 1) std::cout << ", ";
//            }
//            std::cout << "\n";
//        }
    }

    void point_insert(const Point &point) {
        if (simplexes.empty()) {
            std::cerr << "Error: No simplexes in triangulation\n";
            return;
        }

        size_t dimension = point.size();
        std::vector<size_t> bad_simplex_indices;

#pragma omp parallel for default(none) shared(simplexes, point, bad_simplex_indices)
        for (size_t idx = 0; idx < simplexes.size(); ++idx) {
            try {
                auto [center, radius] = circumsphere(simplexes[idx]);
                if (distance(point, center) <= radius * radius + 1e-8) {
#pragma omp critical
                    {
                        bad_simplex_indices.push_back(idx);
                    }
                }
            } catch (const std::exception &e) {
                continue;
            }
        }

        if (bad_simplex_indices.empty()) {
            return;
        }

        std::map<std::vector<Point>, int, FacetComparator> facet_count;

        for (size_t idx: bad_simplex_indices) {
            for (size_t i = 0; i < simplexes[idx].vertices.size(); ++i) {
                std::vector<Point> facet = getFace(simplexes[idx], i);
                std::sort(facet.begin(), facet.end());
                facet_count[facet]++;
            }
        }

        std::vector<std::vector<Point>> boundary_facets;
        for (const auto &[facet, count]: facet_count) {
            if (count == 1) {
                boundary_facets.push_back(facet);
            }
        }

        // Удаляем плохие симплексы в обратном порядке по индексам
        std::sort(bad_simplex_indices.rbegin(), bad_simplex_indices.rend());
        std::vector<Simplex> new_simplexes;
        std::set<size_t> bad_set(bad_simplex_indices.begin(), bad_simplex_indices.end());
        for (size_t i = 0; i < simplexes.size(); ++i) {
            if (!bad_set.count(i)) {
                new_simplexes.push_back(simplexes[i]);
            }
        }
        simplexes = std::move(new_simplexes);

        for (const auto &facet: boundary_facets) {
            std::vector<Point> new_simplex_vertices = facet;
            new_simplex_vertices.push_back(point);

            if (new_simplex_vertices.size() == dimension + 1) {
                simplexes.emplace_back(new_simplex_vertices);
            }
        }

    }

    void delete_super_simplex() {
        if (super_simplex_vertices.empty()) {
            std::cerr << "Warning: No super simplex vertices to delete\n";
            return;
        }

        std::set<Point, PointComparator> super_vertices(
                super_simplex_vertices.begin(),
                super_simplex_vertices.end()
        );

        std::vector<Simplex> new_simplexes;
        new_simplexes.reserve(simplexes.size());

        for (const auto &simplex: simplexes) {
            bool has_super_vertex = false;
            for (const auto &vertex: simplex.vertices) {
                if (super_vertices.count(vertex)) {
                    has_super_vertex = true;
                    break;
                }
            }
            if (!has_super_vertex) {
                new_simplexes.push_back(simplex);
            }
        }

        simplexes = std::move(new_simplexes);
    }

    [[nodiscard]] std::vector<Point> get_points() const {
        std::set<Point, PointComparator> unique_points;
        for (const auto &simplex: simplexes) {
            for (const auto &vertex: simplex.vertices) {
                unique_points.insert(vertex);
            }
        }
        return std::vector<Point>(unique_points.begin(), unique_points.end());
    }
    [[nodiscard]] std::vector<Point> get_points_() const {
        std::vector<Point> points;
        for (const auto &simplex: simplexes) {
            for (const auto &vertex: simplex.vertices) {
                points.push_back(vertex);
            }
        }
        std::sort(points.begin(), points.end());
        points.erase(std::unique(points.begin(), points.end()), points.end());
        return points;
    }

    [[nodiscard]] std::vector<Simplex> get_simplexes() const {
        return simplexes;
    }

    friend std::ostream &operator<<(std::ostream &os, const Delaunay &delaunay) {
        os << "Delaunay Triangulation (" << delaunay.simplexes.size() << " simplexes):\n";
        for (size_t i = 0; i < delaunay.simplexes.size(); ++i) {
            os << "  " << i + 1 << ": " << delaunay.simplexes[i] << "\n";
        }
        return os;
    }
};

#endif //SHGO2_DELAUNAY_H
