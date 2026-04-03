//
// Created by egorm on 16-Mar-26.
//

#ifndef SHGO2_EDGE_H
#define SHGO2_EDGE_H

#include <vector>

using Point = std::vector<double>;

struct Edge {
    Point p1, p2;

    Edge(Point a, Point b) : p1(std::move(a)), p2(std::move(b)) {
        if (p1 > p2) {
            std::swap(p1, p2);
        }
    }

    bool operator==(const Edge &other) const {
        return p1 == other.p1 && p2 == other.p2;
    }

    bool operator<(const Edge &other) const {
        if (p1 != other.p1) return p1 < other.p1;
        return p2 < other.p2;
    }
};

#endif //SHGO2_EDGE_H
