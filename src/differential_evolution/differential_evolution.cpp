//
// Created by egorm on 20-Oct-25.
//
#include "differential_evolution/differential_evolution.h"
#include <random>


OptimizeResult
differential_evolution(const std::function<double(const std::vector<double> &)> &objFunc,
                       const std::vector<std::pair<double, double>> &bounds, const DEOptions& options) {
    std::mt19937 gen(options.random_seed);
    std::uniform_real_distribution<double> dis_real(0.0, 1.0);
    std::uniform_int_distribution<int> dis_index(0, options.popsize - 1);
    int call_count = 0;
    auto function = [&objFunc, &call_count](const std::vector<double> &input) {
        call_count++;
        return objFunc(input);
    };
    size_t input_size = bounds.size();
    vector<vector<double>> population(options.popsize, std::vector<double>(input_size));
    vector<double> fitness(options.popsize);

    for (int i = 0; i < options.popsize; i++) {
        for (int j = 0; j < input_size; ++j) {
            double y_val = bounds[j].first + (bounds[j].second - bounds[j].first) * dis_real(gen);
            population[i][j] = y_val;
        }
        fitness[i] = function(population[i]);
    }

    for (int g = 0; g < options.maxiter; g++) {
        vector<vector<double>> new_population = population;
        vector<double> new_fitness = fitness;

        for (int i = 0; i < options.popsize; i++) {
            int r1, r2, r3;
            do { r1 = dis_index(gen); } while (r1 == i);
            do { r2 = dis_index(gen); } while (r2 == i || r2 == r1);
            do { r3 = dis_index(gen); } while (r3 == i || r3 == r1 || r3 == r2);
            vector<double> trial_vec = population[i];
            for (int j = 0; j < input_size; ++j) {
                double y_mutant = population[r1][j] + options.F * (population[r2][j] - population[r3][j]);
                if (y_mutant < bounds[j].first) y_mutant = bounds[j].first;
                if (y_mutant > bounds[j].second) y_mutant = bounds[j].second;
                if (dis_real(gen) < options.CR) {
                    trial_vec[j] = y_mutant;
                }
            }
            double trial_fitness = function(trial_vec);

            if (trial_fitness < fitness[i]) {
                new_population[i] = trial_vec;
                new_fitness[i] = trial_fitness;
            }
        }

        population = new_population;
        fitness = new_fitness;
    }

    int best_index = 0;
    for (int i = 1; i < options.popsize; i++) {
        if (fitness[i] < fitness[best_index]) {
            best_index = i;
        }
    }
    OptimizeResult result;
    result.x = population[best_index];
    result.fun = fitness[best_index];
    result.nfev = call_count;
    return result;
}
