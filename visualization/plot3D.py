
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter

from functions.function import Function


def create_3d_animation(obj_function: Function, generations, gif_filename,
                        pop_history, fit_history, best_history):
    """Создает анимацию для 2D функций (3D визуализация)"""

    def func(x):
        return obj_function.compute(x)

    bounds = [obj_function.get_low_bound(), obj_function.get_up_bound()]
    x_grid = np.linspace(bounds[0], bounds[1], 50)
    y_grid = np.linspace(bounds[0], bounds[1], 50)
    X, Y = np.meshgrid(x_grid, y_grid)

    Z = np.zeros_like(X)
    for i in range(X.shape[0]):
        for j in range(X.shape[1]):
            Z[i, j] = func(np.array([X[i, j], Y[i, j]]))

    fig = plt.figure(figsize=(16, 8))

    ax1 = fig.add_subplot(121, projection='3d')

    ax2 = fig.add_subplot(122)

    surf = ax1.plot_surface(X, Y, Z, cmap='coolwarm', alpha=0.6,
                            antialiased=True, linewidth=0, rcount=50, ccount=50)

    ax1.set_xlabel('X', fontsize=12)
    ax1.set_ylabel('Y', fontsize=12)
    ax1.set_zlabel('f(X, Y)', fontsize=12)
    ax1.set_title(f'Дифференциальная эволюция: {obj_function.__class__.__name__}\nПоколение: 0/{generations}',
                  fontsize=14, pad=20)

    fig.colorbar(surf, ax=ax1, shrink=0.5, aspect=5, label='Значение функции')

    scatter_pop = ax1.scatter([], [], [], c='red', s=30, alpha=0.8, label='Популяция', depthshade=True)
    scatter_best = ax1.scatter([], [], [], c='green', s=100, marker='*',
                               alpha=1, label='Лучшая особь', depthshade=True)
    ax1.legend(loc='upper right')

    ax2.set_xlim(0, generations)
    ax2.set_ylim(min(best_history) - 0.1 * abs(min(best_history)),
                 max(best_history[:10]) + 0.1 * abs(max(best_history[:10])))
    ax2.set_xlabel('Поколение', fontsize=12)
    ax2.set_ylabel('Лучшее значение', fontsize=12)
    ax2.set_title('Сходимость алгоритма', fontsize=14)
    ax2.grid(True, alpha=0.3)

    line_best, = ax2.plot([], [], 'b-', linewidth=2, label='Лучшее значение')
    scatter_current_best = ax2.scatter([], [], c='red', s=50, zorder=5, label='Текущее поколение')
    ax2.legend(loc='upper right')

    info_text = ax2.text(0.02, 0.98, '', transform=ax2.transAxes,
                         verticalalignment='top', fontsize=10,
                         bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

    def init():
        """Инициализация анимации"""
        scatter_pop._offsets3d = ([], [], [])
        scatter_best._offsets3d = ([], [], [])
        line_best.set_data([], [])
        scatter_current_best.set_offsets(np.empty((0, 2)))
        info_text.set_text('')
        return scatter_pop, scatter_best, line_best, scatter_current_best, info_text

    def update(frame):
        """Обновление кадра анимации"""
        current_pop = pop_history[frame]
        current_fit = fit_history[frame]

        if len(current_fit.shape) > 1:
            current_fit = current_fit.flatten()

        x_pop = current_pop[:, 0]
        y_pop = current_pop[:, 1]
        z_pop = current_fit

        best_idx = np.argmin(current_fit)
        best_coords = ([current_pop[best_idx, 0]],
                       [current_pop[best_idx, 1]],
                       [current_fit[best_idx]])

        scatter_pop._offsets3d = (x_pop, y_pop, z_pop)
        scatter_best._offsets3d = best_coords

        ax1.set_title(f'Дифференциальная эволюция: {obj_function.__class__.__name__}\nПоколение: {frame}/{generations}',
                      fontsize=14, pad=20)

        x_history = list(range(frame + 1))
        y_history = best_history[:frame + 1]

        line_best.set_data(x_history, y_history)
        scatter_current_best.set_offsets([[frame, best_history[frame]]])

        info_text.set_text(f'Поколение: {frame}\n'
                           f'Лучшее значение: {best_history[frame]:.6f}\n'
                           f'Лучшая точка: ({current_pop[best_idx, 0]:.3f}, {current_pop[best_idx, 1]:.3f})')

        if frame > 0:
            ax2.set_ylim(min(best_history[:frame + 1]) - 0.1 * abs(min(best_history[:frame + 1])),
                         max(best_history[:min(frame + 1, 10)]) + 0.1 * abs(max(best_history[:min(frame + 1, 10)])))

        return scatter_pop, scatter_best, line_best, scatter_current_best, info_text

    ani = FuncAnimation(fig, update, frames=min(generations + 1, len(pop_history)),
                        init_func=init, blit=False, repeat=False,
                        interval=200)

    print(f"Сохранение GIF как '{gif_filename}'...")

    if len(pop_history) > 0:
        ani.save(gif_filename, writer=PillowWriter(fps=5))
        print(f"Анимация сохранена!")
    else:
        print("Ошибка: нет данных для анимации")

    plt.close(fig)
    return ani
