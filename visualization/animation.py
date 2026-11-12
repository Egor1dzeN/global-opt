from differential_evolution import differential_evolution
from functions.Hill import Hill
from functions.function import Function
from plot2D import create_2d_animation
from plot3D import create_3d_animation


def create_de_animation(obj_function: Function,
                        generations: int = 50,
                        pop_size: int = 20,
                        gif_filename: str = 'de_evolution.gif',
                        e: float = 1e-7):
    """
    Create an animation of the differential evolution process.
    Supports 1D (2D plot) and 2D (3D plot) functions.

    Parameters:
    -----------
    obj_function : Function
        Objective function
    generations : int
        Number of generations
    pop_size : int
        Population size
    gif_filename : str
        Output GIF file name
    """

    input_dim = obj_function.get_dim()

    def func(x):
        return obj_function.compute(x)

    best_val, best_solution, population_history, fitness_history, best_history = differential_evolution(
        obj_func=func,
        input_size=input_dim,
        min_y=obj_function.get_low_bound(),
        max_y=obj_function.get_up_bound(),
        count_generation=generations,
        np_size=pop_size,
        random_seed=42
    )
    for i, pop in enumerate(best_history):
        if abs(pop - best_val) < e:
            print(f'Converged at generation {i} of {generations}')
            break
    else:
        print(f'Did not converge within {generations} generations')
    print(f"Minimizer found: {best_solution}")
    print(f"Minimum value: {best_val:.6f}")

    if input_dim == 1:
        return create_2d_animation(obj_function, generations, gif_filename,
                                   population_history, fitness_history, best_history)
    elif input_dim == 2:
        return create_3d_animation(obj_function, generations, gif_filename,
                                   population_history, fitness_history, best_history)
    else:
        raise ValueError(f"The function dimension is supported only by 1 or 2.Your dim is {input_dim}")


if __name__ == '__main__':
    hill = Hill()
    create_de_animation(hill)
