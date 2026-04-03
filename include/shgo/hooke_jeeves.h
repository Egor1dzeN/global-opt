//
// Created by egorm on 23-Mar-26.
//

#ifndef SHGO2_HOOKE_JEEVES_H
#define SHGO2_HOOKE_JEEVES_H

#include <utility>

#include "vector"
#include "functional"
#include <cmath>

using Point = std::vector<double>;
using pdd = std::pair<double, double>;

class HookeJeevesMethod {
private:
    double mStep;
    double mEps;
    double mStepMultiplier;
    int mMaxIterations;
    int mIterationsCount;
    std::vector<pdd> mBounds;
    std::function<double(Point)> func;

    void clampToBounds(Point &point) const {
        for (size_t i = 0; i < point.size() && i < mBounds.size(); ++i) {
            if (point[i] < mBounds[i].first) {
                point[i] = mBounds[i].first;
            } else if (point[i] > mBounds[i].second) {
                point[i] = mBounds[i].second;
            }
        }
    }

    [[nodiscard]] bool isWithinBounds(const Point &point) const {
        for (size_t i = 0; i < point.size() && i < mBounds.size(); ++i) {
            if (point[i] < mBounds[i].first || point[i] > mBounds[i].second) {
                return false;
            }
        }
        return true;
    }

    double evaluateFunction(const Point &x) {
        if (!isWithinBounds(x)) {
            return HUGE_VAL;
        }
        return func(x);
    }

    Point exploratorySearch(const Point &startPoint, double step, bool &improved) {
        Point currentPoint = startPoint;
        double currentValue = evaluateFunction(currentPoint);
        improved = false;

        if (currentValue == HUGE_VAL) {
            return currentPoint;
        }

        for (size_t i = 0; i < currentPoint.size(); ++i) {
            Point testPoint = currentPoint;
            testPoint[i] += step;
            clampToBounds(testPoint);
            double testValue = evaluateFunction(testPoint);

            if (testValue < currentValue) {
                currentPoint = testPoint;
                currentValue = testValue;
                improved = true;
                continue;
            }

            testPoint = currentPoint;
            testPoint[i] -= step;
            clampToBounds(testPoint);
            testValue = evaluateFunction(testPoint);

            if (testValue < currentValue) {
                currentPoint = testPoint;
                currentValue = testValue;
                improved = true;
            }
        }

        return currentPoint;
    }

public:
    HookeJeevesMethod(std::function<double(Point)> func,
                      const std::vector<pdd> &bounds,
                      double initialStep = 0.1,
                      double epsilon = 1e-6,
                      double stepMultiplier = 2.0,
                      int maxIterations = 1000)
            : func(std::move(func)),
              mBounds(bounds),
              mStep(initialStep),
              mEps(epsilon),
              mStepMultiplier(stepMultiplier),
              mMaxIterations(maxIterations),
              mIterationsCount(0) {}

    explicit HookeJeevesMethod(double initialStep = 0.1,
                               double epsilon = 1e-6,
                               double stepMultiplier = 2.0,
                               int maxIterations = 1000)
            : mStep(initialStep),
              mEps(epsilon),
              mStepMultiplier(stepMultiplier),
              mMaxIterations(maxIterations),
              mIterationsCount(0) {}

    Point optimize(const Point &startPoint) {
        Point basePoint = startPoint;

        if (!mBounds.empty()) {
            clampToBounds(basePoint);
        }

        double baseValue = evaluateFunction(basePoint);

        if (baseValue == HUGE_VAL && !mBounds.empty()) {
            for (size_t i = 0; i < basePoint.size(); ++i) {
                basePoint[i] = mBounds[i].first;
            }
            baseValue = evaluateFunction(basePoint);

            if (baseValue == HUGE_VAL) {
                return startPoint;
            }
        }

        Point currentPoint = basePoint;
        Point previousPoint = basePoint;

        mIterationsCount = 0;
        double currentStep = mStep;

        while (currentStep > mEps && mIterationsCount < mMaxIterations) {
            mIterationsCount++;

            bool improved;
            Point newPoint = exploratorySearch(currentPoint, currentStep, improved);
            double newValue = evaluateFunction(newPoint);

            if (newValue < baseValue) {
                previousPoint = currentPoint;
                currentPoint = newPoint;
                basePoint = newPoint;
                baseValue = newValue;

                for (size_t i = 0; i < currentPoint.size(); ++i) {
                    currentPoint[i] = currentPoint[i] + (currentPoint[i] - previousPoint[i]);
                }

                if (!mBounds.empty()) {
                    clampToBounds(currentPoint);
                }
            } else {
                currentStep /= mStepMultiplier;
            }
        }

        bool improved;
        Point finalPoint = exploratorySearch(basePoint, mEps, improved);
        if (improved) {
            basePoint = finalPoint;
        }

        return basePoint;
    }

    [[nodiscard]] int getIterationsCount() const { return mIterationsCount; }

    void setBounds(const std::vector<pdd> &bounds) {
        mBounds = bounds;
    }
};

#endif //SHGO2_HOOKE_JEEVES_H
