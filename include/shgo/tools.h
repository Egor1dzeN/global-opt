//
// Created by egorm on 16-Mar-26.
//

#ifndef SHGO2_TOOLS_H
#define SHGO2_TOOLS_H

#include <vector>
#include <iostream>

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    os << "(";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1) {
            os << ", ";
        }
    }
    os << ")";
    return os;
}

#endif //SHGO2_TOOLS_H
