import random
from typing import List, Callable

import numpy as np

from functions.Hill import Hill


def differential_evolution(
        obj_func: Callable[[List[float]], float],
        input_size: int,
        min_y: float,
        max_y: float,
        count_generation: int = 100,
        np_size: int = 40,
        F: float = 0.8,
        CR: float = 0.9,
        random_seed: int = 42
):
    """
    Дифференциальная эволюция с сохранением истории популяции и fitness.
    
    Возвращает:
    - лучший результат
    - историю популяции на каждой итерации
    - историю fitness на каждой итерации
    """
    np.random.seed(random_seed)
    random.seed(random_seed)

    population = np.random.uniform(min_y, max_y, (np_size, input_size))
    fitness = np.array([obj_func(ind.tolist()) for ind in population])

    population_history = [population.copy()]
    fitness_history = [fitness.copy()]
    best_history = [np.min(fitness)]

    for g in range(count_generation):
        new_population = population.copy()
        new_fitness = fitness.copy()

        for i in range(np_size):
            candidates = [idx for idx in range(np_size) if idx != i]
            r1, r2, r3 = np.random.choice(candidates, 3, replace=False)

            trial = population[i].copy()

            j_rand = random.randint(0, input_size - 1)

            for j in range(input_size):
                if random.random() < CR or j == j_rand:
                    mutant = population[r1][j] + F * (population[r2][j] - population[r3][j])

                    if mutant < min_y:
                        mutant = min_y
                    elif mutant > max_y:
                        mutant = max_y

                    trial[j] = mutant

            trial_fitness = obj_func(trial.tolist())

            if trial_fitness < fitness[i]:
                new_population[i] = trial
                new_fitness[i] = trial_fitness

        population = new_population
        fitness = new_fitness

        population_history.append(population.copy())
        fitness_history.append(fitness.copy())
        best_history.append(np.min(fitness))

    best_idx = np.argmin(fitness)
    fitness = fitness.flatten()
    return (float(fitness[best_idx]),
            population[best_idx].tolist(),
            population_history,
            fitness_history,
            best_history)

if __name__ == "__main__":
    hill = Hill()
    # create_plot(hill)
    print(differential_evolution(obj_func=hill.compute, input_size=1, min_y=0, max_y=1))
