//
// Created by egorm on 20-Oct-25.
//
#include <DE_finding_global_min.h>


const int NP = 40;
const double F = 0.8;
const double CR = 0.9;
const double min_y = -1.0;
const double max_y = 1.0;
const int max_generations = 200;


std::mt19937 gen(std::random_device{}());
std::uniform_real_distribution<double> dis_real(0.0, 1.0);
std::uniform_int_distribution<int> dis_index(0, NP - 1);

std::pair<double, double>
findGlobalMinimum(const std::function<double(const std::vector<double> &)> &objFunc, size_t input_size) {
    vector<vector<double>> population(NP, std::vector<double>(input_size));
    vector<double> fitness(NP);

    // Инициализация популяции (2D векторы)
    for (int i = 0; i < NP; i++) {
        for (int j = 0; j < input_size; ++j) {
            double y_val = min_y + (max_y - min_y) * dis_real(gen);
            population[i][j] = y_val;
        }
//        population[i] = {y_val1, y_val2};
        fitness[i] = objFunc(population[i]);
    }

    for (int g = 0; g < max_generations; g++) {
        vector<vector<double>> new_population = population;
        vector<double> new_fitness = fitness;

        for (int i = 0; i < NP; i++) {
            // Выбор трех различных особей
            int r1, r2, r3;
            do { r1 = dis_index(gen); } while (r1 == i);
            do { r2 = dis_index(gen); } while (r2 == i || r2 == r1);
            do { r3 = dis_index(gen); } while (r3 == i || r3 == r1 || r3 == r2);
            vector<double> trial_vec = population[i];
            // Мутация для обеих координат
            for (int j = 0; j < input_size; ++j) {
                double y_mutant = population[r1][j] + F * (population[r2][j] - population[r3][j]);
                if (y_mutant < min_y) y_mutant = min_y;
                if (y_mutant > max_y) y_mutant = max_y;
                if (dis_real(gen) < CR) {
                    trial_vec[j] = y_mutant;
                }
            }
            double trial_fitness = objFunc(trial_vec);

            // Отбор
            if (trial_fitness < fitness[i]) {
                new_population[i] = trial_vec;
                new_fitness[i] = trial_fitness;
            }
            // else - уже скопировано в new_population[i] = population[i]
        }

        population = new_population;
        fitness = new_fitness;
    }

    // Поиск лучшего решения
    int best_index = 0;
    for (int i = 1; i < NP; i++) {
        if (fitness[i] < fitness[best_index]) {
            best_index = i;
        }
    }

    // Возвращаем обе координаты и значение функции
    return std::make_pair(fitness[best_index], population[best_index][0]);
}
