//
// Created by egorm on 16-Mar-26.
//

#ifndef SHGO2_SIMPLEX_H
#define SHGO2_SIMPLEX_H

#include <algorithm>
#include <vector>
#include "tools.h"

using Point = std::vector<double>;

struct Simplex {
    std::vector<Point> vertices;

    Simplex(const std::vector<Point> &verts) : vertices(verts) {}

    Simplex(std::initializer_list<Point> verts) : vertices(verts) {}

    bool operator==(const Simplex &other) const {
        if (vertices.size() != other.vertices.size()) return false;

        std::vector<Point> points1 = vertices;
        std::vector<Point> points2 = other.vertices;

        std::sort(points1.begin(), points1.end());
        std::sort(points2.begin(), points2.end());

        for (size_t i = 0; i < points1.size(); ++i) {
            if (points1[i] != points2[i]) return false;
        }
        return true;
    }

    [[nodiscard]] bool containsVertex(const Point &point) const {
        for (const auto &v: vertices) {
            if (v == point) return true;
        }
        return false;
    }

    [[nodiscard]] size_t getDimension() const {
        return vertices.empty() ? 0 : vertices[0].size();
    }

    std::vector<Point> getVertices() const{
        return vertices;
    }

    friend std::ostream &operator<<(std::ostream &os, const Simplex &simplex) {
        os << "Simplex(";
        for (size_t i = 0; i < simplex.vertices.size(); ++i) {
            os << "v" << i << ": " << simplex.vertices[i];
            if (i < simplex.vertices.size() - 1) os << ", ";
        }
        os << ")";
        return os;
    }
};

#endif //SHGO2_SIMPLEX_H
