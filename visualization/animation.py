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
    Создает анимацию процесса дифференциальной эволюции.
    Поддерживает как 2D, так и 3D функции.

    Parameters:
    -----------
    obj_function : Function
        Оптимизируемая функция
    generations : int
        Количество шагов итерации
    pop_size : int
        Размер популяции
    gif_filename : str
        Имя файла для сохранения GIF
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
            print(f'Сходится на {i} шаге из {generations}')
            break
    else:
        print(f'Алгоритм не сошелся при {generations} шагах')
    print(f"Найденный минимум: func({best_solution})")
    print(f"Значение в минимуме: {best_val:.6f}")

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
