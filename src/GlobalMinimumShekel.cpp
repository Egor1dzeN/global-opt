//
// Created by egorm on 20-Oct-25.
//
//
// Created by egorm on 20-Oct-25.
//
#include <GlobalMinimumShekel.h>


const int NP = 40;
const double F = 0.8;
const double CR = 0.9;
const double min_y = -10.0;
const double max_y = 10.0;
const int max_generations = 200;


std::mt19937 gen(std::random_device{}());
std::uniform_real_distribution<double> dis_real(0.0, 1.0);
std::uniform_int_distribution<int> dis_index(0, NP - 1);

std::pair<double, double> findGlobalMinimum(int problemIndex, const std::function<double(const vector<double> &)> &objFunc) {
    vector<vector<double>> population(NP);
    vector<double> fitness(NP);

    for (int i = 0; i < NP; i++) {
        double y_val = min_y + (max_y - min_y) * dis_real(gen);
        population[i] = {y_val};
        fitness[i] = objFunc(population[i]);
    }

    for (int g = 0; g < max_generations; g++) {

        vector<vector<double>> new_population = population;
        vector<double> new_fitness = fitness;

        for (int i = 0; i < NP; i++) {

            int r1, r2, r3;
            do { r1 = dis_index(gen); } while (r1 == i);
            do { r2 = dis_index(gen); } while (r2 == i || r2 == r1);
            do { r3 = dis_index(gen); } while (r3 == i || r3 == r1 || r3 == r2);

            // v = a + F * (b - c)
            double y_mutant = population[r1][0] + F * (population[r2][0] - population[r3][0]);

            if (y_mutant < min_y) y_mutant = min_y;
            if (y_mutant > max_y) y_mutant = max_y;

            double y_trial = population[i][0];

            // (rand() < CR) ->
            if (dis_real(gen) < CR) {
                y_trial = y_mutant;
            }

            vector<double> trial_vec = {y_trial};

            double trial_fitness = objFunc(trial_vec);

            if (trial_fitness < fitness[i]) {
                new_population[i] = trial_vec;
                new_fitness[i] = trial_fitness;
            } else {
                new_population[i] = population[i];
                new_fitness[i] = fitness[i];
            }
        }

        population = new_population;
        fitness = new_fitness;
    }

    double best_y = population[0][0];
    double best_fitness = fitness[0];

    for (int i = 1; i < NP; i++) {
        if (fitness[i] < best_fitness) {
            best_fitness = fitness[i];
            best_y = population[i][0];
        }
    }

    std::cout << "Global min (nearly): f(" << best_y << ") = " << best_fitness << std::endl;
    return std::make_pair(best_fitness, best_y);
}
