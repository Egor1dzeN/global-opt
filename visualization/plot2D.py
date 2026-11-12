import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation, PillowWriter
from matplotlib.collections import LineCollection


def create_2d_animation(obj_function, generations, gif_filename,
                        population_history, fitness_history, best_history):
    """Create an animation for 1D functions."""

    def func(x):
        return obj_function.compute(x)

    x_func = np.linspace(obj_function.get_low_bound(), obj_function.get_up_bound(), 400)
    y_func = func(x_func)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

    points = np.array([x_func, y_func]).T.reshape(-1, 1, 2)
    segments = np.concatenate([points[:-1], points[1:]], axis=1)
    lc = LineCollection(segments, cmap='coolwarm', norm=plt.Normalize(y_func.min(), y_func.max()))
    lc.set_array(y_func[:-1])
    lc.set_linewidth(2)
    ax1.add_collection(lc)

    ax1.set_xlim(x_func.min(), x_func.max())
    ax1.set_ylim(y_func.min() - 0.1 * abs(y_func.min()), y_func.max() + 0.1 * abs(y_func.max()))
    ax1.set_xlabel('X', fontsize=12)
    ax1.set_ylabel('Y', fontsize=12)
    ax1.set_title(f'Differential evolution: {obj_function.__class__.__name__}\nGeneration: 0/{generations}',
                  fontsize=14)
    ax1.grid(True, alpha=0.3)

    cbar1 = plt.colorbar(lc, ax=ax1)
    cbar1.set_label('Function value', fontsize=10)

    scatter_pop = ax1.scatter([], [], c='red', s=50, alpha=0.7, label='Population', zorder=5)
    scatter_best = ax1.scatter([], [], c='green', s=100, marker='*', alpha=1, label='Best individual', zorder=6)
    ax1.legend(loc='upper right')

    ax2.set_xlim(0, generations)
    ax2.set_ylim(min(best_history) - 0.1 * abs(min(best_history)),
                 max(best_history[:10]) + 0.1 * abs(max(best_history[:10])))
    ax2.set_xlabel('Generation', fontsize=12)
    ax2.set_ylabel('Best value', fontsize=12)
    ax2.set_title('Convergence', fontsize=14)
    ax2.grid(True, alpha=0.3)

    line_best, = ax2.plot([], [], 'b-', linewidth=2, label='Best value')
    scatter_current_best = ax2.scatter([], [], c='red', s=50, zorder=5, label='Current generation')
    ax2.legend(loc='upper right')

    info_text = ax2.text(0.02, 0.98, '', transform=ax2.transAxes,
                         verticalalignment='top', fontsize=10,
                         bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

    def init():
        """Initialize the animation."""
        scatter_pop.set_offsets(np.empty((0, 2)))
        scatter_best.set_offsets(np.empty((0, 2)))
        line_best.set_data([], [])
        scatter_current_best.set_offsets(np.empty((0, 2)))
        info_text.set_text('')
        return scatter_pop, scatter_best, line_best, scatter_current_best, info_text

    def update(frame):
        """Update an animation frame."""
        current_pop = population_history[frame]
        current_fit = fitness_history[frame]

        if len(current_fit.shape) > 1:
            current_fit = current_fit.flatten()

        pop_coords = np.zeros((len(current_pop), 2))
        pop_coords[:, 0] = current_pop[:, 0]
        pop_coords[:, 1] = current_fit

        best_idx = np.argmin(current_fit)
        best_coord = np.array([[current_pop[best_idx, 0], current_fit[best_idx]]])

        scatter_pop.set_offsets(pop_coords)
        scatter_best.set_offsets(best_coord)

        ax1.set_title(f'Differential evolution: {obj_function.__class__.__name__}\nGeneration: {frame}/{generations}',
                      fontsize=14)

        x_history = list(range(frame + 1))
        y_history = best_history[:frame + 1]

        line_best.set_data(x_history, y_history)
        scatter_current_best.set_offsets([[frame, best_history[frame]]])

        info_text.set_text(f'Generation: {frame}\n'
                           f'Best value: {best_history[frame]:.6f}\n'
                           f'Best x: {current_pop[best_idx, 0]:.4f}')
        if best_history[frame] < -4.6:  # target value reached
            line_best.set_color('green')
        elif best_history[frame] < 15:
            line_best.set_color('orange')
        else:
            line_best.set_color('b')
        if frame > 0:
            ax2.set_ylim(min(best_history[:frame + 1]) - 0.1 * abs(min(best_history[:frame + 1])),
                         max(best_history[:min(frame + 1, 10)]) + 0.1 * abs(max(best_history[:min(frame + 1, 10)])))

        return scatter_pop, scatter_best, line_best, scatter_current_best, info_text

    ani = FuncAnimation(fig, update, frames=min(generations + 1, len(population_history)),
                        init_func=init, blit=False, repeat=False,
                        interval=200)

    print(f"Saving GIF to '{gif_filename}'...")

    if len(population_history) > 0:
        ani.save(gif_filename, writer=PillowWriter(fps=5))
        print(f"Animation saved")
    else:
        print("Error: no data to animate")

    plt.close(fig)
    return ani
