import sys
import numpy as np
from PyQt6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout,
                             QHBoxLayout, QComboBox, QPushButton, QLabel, QSlider,
                             QCheckBox)
from PyQt6.QtCore import Qt
import matplotlib

matplotlib.use('QtAgg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib import cm
from matplotlib.colors import LogNorm

try:
    import global_opt_py
except ImportError:
    raise Exception("Error: import global_opt_py")

from problems.hill import Hill
from problems.shekel import Shekel
from problems.grishagin_function.grishagin_function import GrishaginFunction
from problems.GKLS_function.gkls_function import GKLSFunction, GKLSClass
from iOpt.trial import Point, FunctionValue


# --- 1D functions ---
def quadratic_1d(x):
    """Quadratic function: f(x) = x^2"""
    return x[0] ** 2


def sine_1d(x):
    """Sine function: f(x) = sin(x) + 0.1*x"""
    return np.sin(x[0]) + 0.1 * x[0]


def polynomial_1d(x):
    """Fourth-degree polynomial: f(x) = x^4 - 2x^2 + 0.5x"""
    return x[0] ** 4 - 2 * x[0] ** 2 + 0.5 * x[0]


def exponential_1d(x):
    """Exponential function: f(x) = x * exp(-x^2/10)"""
    return x[0] * np.exp(-x[0] ** 2 / 10)


def multimodal_1d(x):
    """Multimodal function: f(x) = sin(x) + sin(3x) + 0.3x"""
    return np.sin(x[0]) + np.sin(3 * x[0]) + 0.3 * x[0]


# --- 2D functions ---
def sphere(x):
    return sum(xi ** 2 for xi in x)


def rastrigin(x):
    n = len(x)
    A = 10
    return A * n + sum([(xi ** 2 - A * np.cos(2 * np.pi * xi)) for xi in x])


def rosenbrock(x):
    if len(x) < 2:
        return (1 - x[0]) ** 2
    return sum(100.0 * (x[i + 1] - x[i] ** 2) ** 2 + (1 - x[i]) ** 2 for i in range(len(x) - 1))


def ackley(x):
    n = len(x)
    a = 20
    b = 0.2
    c = 2 * np.pi
    sum_sq = sum(xi ** 2 for xi in x)
    sum_cos = sum(np.cos(c * xi) for xi in x)
    term1 = -a * np.exp(-b * np.sqrt(sum_sq / n))
    term2 = -np.exp(sum_cos / n)
    return term1 + term2 + a + np.e


# --- Hill function wrapper ---
class HillFunctionWrapper:
    """Wrapper for Hill function number 1."""

    def __init__(self):
        self.function_number = 1
        self.hill_problem = Hill(self.function_number)

        # Known global minimum
        true_trial = self.hill_problem.known_optimum[0]
        self.true_value = true_trial.function_values[0].value
        self.true_x = true_trial.point.float_variables[0]

    def __call__(self, x):
        """Evaluate the Hill function at x."""
        point = Point(np.array([x[0]], dtype=np.double), None)
        fv = FunctionValue()
        result = self.hill_problem.calculate(point, fv)
        return result.value


# --- Shekel function wrapper ---
class ShekelFunctionWrapper:
    """Wrapper for Shekel function number 1."""

    def __init__(self):
        self.function_number = 1
        self.shekel_problem = Shekel(self.function_number)

        # Known global minimum
        true_trial = self.shekel_problem.known_optimum[0]
        self.true_value = true_trial.function_values[0].value
        self.true_x = true_trial.point.float_variables[0]

    def __call__(self, x):
        """Evaluate the Shekel function at x."""
        point = Point(np.array([x[0]], dtype=np.double), None)
        fv = FunctionValue()
        result = self.shekel_problem.calculate(point, fv)
        return result.value


# --- Grishagin function wrapper ---
class GrishaginFunctionWrapper:
    """Wrapper for Grishagin function number 1."""

    def __init__(self):
        self.function_number = 1
        self.grishagin_problem = GrishaginFunction(self.function_number)

        # Known global minimum
        self.true_point = self.grishagin_problem.GetOptimumPoint()
        self.true_value = self.grishagin_problem.Calculate(self.true_point)
        self.true_x1 = self.true_point[0]
        self.true_x2 = self.true_point[1]

    def __call__(self, x):
        """Evaluate the Grishagin function at x (2D)."""
        return self.grishagin_problem.Calculate(x)


# --- GKLS function wrapper ---
class GKLSFunctionWrapper:
    """Wrapper for GKLS function number 1 (Simple class, 2D)."""

    def __init__(self):
        self.dimension = 2
        self.function_number = 1
        self.function_class = GKLSClass.Simple

        # Create and configure the GKLS function
        self.gkls_problem = GKLSFunction()
        self.gkls_problem.SetDimension(self.dimension)
        self.gkls_problem.SetFunctionClass(self.function_class, self.dimension)
        self.gkls_problem.SetFunctionNumber(self.function_number)

        # Known global minimum
        self.true_point = self.gkls_problem.GetOptimumPoint()
        self.true_value = self.gkls_problem.Calculate(self.true_point)
        self.true_x1 = self.true_point[0]
        self.true_x2 = self.true_point[1]

        # GKLS bounds
        self.bounds = [(-1.0, 1.0), (-1.0, 1.0)]

    def __call__(self, x):
        """Evaluate the GKLS function at x (2D)."""
        return self.gkls_problem.Calculate(x)


# Function instances
hill_func = HillFunctionWrapper()
shekel_func = ShekelFunctionWrapper()
grishagin_func = GrishaginFunctionWrapper()
gkls_func = GKLSFunctionWrapper()

# Available functions with their dimensions
AVAILABLE_FUNCTIONS = {
    "Quadratic (1D)": {"func": quadratic_1d, "dim": 1, "bounds": [(-5, 5)]},
    "Sine (1D)": {"func": sine_1d, "dim": 1, "bounds": [(-5, 5)]},
    "Polynomial (1D)": {"func": polynomial_1d, "dim": 1, "bounds": [(-3, 3)]},
    "Exponential (1D)": {"func": exponential_1d, "dim": 1, "bounds": [(-4, 4)]},
    "Multimodal (1D)": {"func": multimodal_1d, "dim": 1, "bounds": [(-5, 5)]},
    "Sphere (2D)": {"func": sphere, "dim": 2, "bounds": [(-5, 5), (-5, 5)]},
    "Rastrigin (2D)": {"func": rastrigin, "dim": 2, "bounds": [(-5, 5), (-5, 5)]},
    "Rosenbrock (2D)": {"func": rosenbrock, "dim": 2, "bounds": [(-2, 2), (-2, 2)]},
    "Ackley (2D)": {"func": ackley, "dim": 2, "bounds": [(-5, 5), (-5, 5)]},
    "Hill 1 (1D)": {"func": hill_func, "dim": 1, "bounds": [(0., 1.)],
                    "true_value": hill_func.true_value, "true_x": hill_func.true_x},
    "Shekel 1 (1D)": {"func": shekel_func, "dim": 1, "bounds": [(0., 10.)],
                      "true_value": shekel_func.true_value, "true_x": shekel_func.true_x},
    "Grishagin 1 (2D)": {"func": grishagin_func, "dim": 2, "bounds": [(0., 1.), (0., 1.)],
                         "true_value": grishagin_func.true_value,
                         "true_x": (grishagin_func.true_x1, grishagin_func.true_x2)},
    "GKLS Simple 1 (2D)": {"func": gkls_func, "dim": 2, "bounds": gkls_func.bounds,
                           "true_value": gkls_func.true_value,
                           "true_x": (gkls_func.true_x1, gkls_func.true_x2)}
}

# Available optimization methods
AVAILABLE_METHODS = {
    "Differential Evolution": global_opt_py.differential_evolution,
    "SHGO": global_opt_py.shgo
}


def get_count_points(dim: int) -> int:
    """
    Return the number of points for the given problem dimension.

    :param dim: problem dimension
    :return: number of points used by the optimizer
    """
    if dim == 1:
        return 1000
    elif dim == 2:
        return 1000
    else:  # dim >= 3
        return 500


class MplCanvas(FigureCanvas):
    """Matplotlib canvas embedded in a PyQt widget."""

    def __init__(self, parent=None, width=6, height=5, dpi=100):
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = self.fig.add_subplot(111)
        super(MplCanvas, self).__init__(self.fig)


class OptimizerWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Global Optimizer UI")
        self.setGeometry(100, 100, 1000, 700)

        # Central widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QHBoxLayout(central_widget)

        # --- Left side: plot ---
        self.canvas = MplCanvas(self, width=7, height=6, dpi=100)
        layout.addWidget(self.canvas, stretch=4)

        # --- Right side: controls ---
        control_panel = QWidget()
        control_layout = QVBoxLayout(control_panel)
        layout.addWidget(control_panel, stretch=1)

        # Function selection
        control_layout.addWidget(QLabel("<b>Function:</b>"))
        self.func_combo = QComboBox()
        self.func_combo.addItems(AVAILABLE_FUNCTIONS.keys())
        self.func_combo.currentTextChanged.connect(self.on_function_change)
        control_layout.addWidget(self.func_combo)

        # Method selection
        control_layout.addWidget(QLabel("<b>Method:</b>"))
        self.method_combo = QComboBox()
        self.method_combo.addItems(AVAILABLE_METHODS.keys())
        control_layout.addWidget(self.method_combo)

        # Number of points (depends on the dimension)
        self.points_info_label = QLabel()
        self.points_info_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.points_info_label.setStyleSheet("QLabel { background-color: #e0e0e0; padding: 5px; border-radius: 3px; }")
        control_layout.addWidget(self.points_info_label)

        # Grid toggle
        self.grid_checkbox = QCheckBox("Show grid")
        self.grid_checkbox.setChecked(True)
        self.grid_checkbox.stateChanged.connect(self.toggle_grid)
        control_layout.addWidget(self.grid_checkbox)

        # Run button
        self.run_btn = QPushButton("Run optimization")
        self.run_btn.clicked.connect(self.run_optimization)
        control_layout.addWidget(self.run_btn)

        # Result
        self.result_label = QLabel("Result: ...")
        self.result_label.setWordWrap(True)
        self.result_label.setStyleSheet("QLabel { background-color: #f0f0f0; padding: 5px; border-radius: 3px; }")
        control_layout.addWidget(self.result_label)

        control_layout.addStretch()

        # Initial state
        self.cbar = None
        self.current_func_info = None
        self.plot_function(list(AVAILABLE_FUNCTIONS.keys())[0])
        self.update_points_info()

    def update_points_info(self):
        """Update the point count label for the current function."""
        func_name = self.func_combo.currentText()
        func_info = AVAILABLE_FUNCTIONS[func_name]
        dim = func_info["dim"]
        count_points = get_count_points(dim)

        # Extra information for functions with a known minimum
        if "true_value" in func_info:
            if dim == 1:
                self.points_info_label.setText(
                    f"Points: {count_points}\n"
                    f"Known minimum: {func_info['true_value']:.6f}\nPoint: {func_info['true_x']:.6f}"
                )
            else:  # 2D
                true_x1, true_x2 = func_info['true_x']
                self.points_info_label.setText(
                    f"Points: {count_points}\n"
                    f"Known minimum: {func_info['true_value']:.6f}\n"
                    f"Point: ({true_x1:.6f}, {true_x2:.6f})"
                )
        else:
            self.points_info_label.setText(f"Points: {count_points}")

    def toggle_grid(self, state):
        """Toggle the plot grid."""
        self.canvas.axes.grid(state == Qt.CheckState.Checked.value)
        self.canvas.draw()

    def on_function_change(self, name):
        self.update_points_info()
        self.plot_function(name, min_point=None)

    def plot_function(self, func_name, min_point=None):
        """Plot the function (1D or 2D)."""
        # Function metadata
        func_info = AVAILABLE_FUNCTIONS[func_name]
        func = func_info["func"]
        dim = func_info["dim"]
        bounds = func_info["bounds"]

        self.current_func_info = func_info

        # Recreate the figure
        self.canvas.fig.clear()
        self.canvas.axes = self.canvas.fig.add_subplot(111)
        self.cbar = None

        # Functions with a known minimum
        true_min_point = None
        if "true_x" in func_info:
            true_min_point = func_info["true_x"]

        if dim == 1:
            # 1D plot
            x_range = bounds[0]
            resolution = 1000
            x_vals = np.linspace(x_range[0], x_range[1], resolution)
            y_vals = [func([x]) for x in x_vals]

            # Function curve
            self.canvas.axes.plot(x_vals, y_vals, linewidth=2, color='blue', label=func_name)
            self.canvas.axes.fill_between(x_vals, y_vals, alpha=0.3, color='blue')

            # Axes setup
            self.canvas.axes.set_xlabel("x", fontsize=12)
            self.canvas.axes.set_ylabel("f(x)", fontsize=12)
            self.canvas.axes.set_title(f"{func_name}", fontsize=14, fontweight='bold')

            if self.grid_checkbox.isChecked():
                self.canvas.axes.grid(True, alpha=0.3, linestyle='--')

            self.canvas.axes.axhline(y=0, color='black', linestyle='-', linewidth=0.5, alpha=0.5)
            self.canvas.axes.axvline(x=0, color='black', linestyle='-', linewidth=0.5, alpha=0.5)

            info_text = ""
            if true_min_point is not None:
                true_y = func_info.get('true_value', func([true_min_point]))
                self.canvas.axes.plot(true_min_point, true_y, 'g*', markersize=12)
                info_text += f"Known minimum:\nf(x) = {true_y:.6f}\nx = {true_min_point:.6f}\n"

            if min_point is not None:
                mx = min_point[0]
                my = func([mx])
                self.canvas.axes.plot(mx, my, 'r*', markersize=15)
                info_text += f"\nFound minimum:\nf(x) = {my:.6f}\nx = {mx:.6f}"

            if info_text:
                self.canvas.axes.text(0.02, 0.98, info_text, transform=self.canvas.axes.transAxes,
                                     verticalalignment='top', fontsize=9,
                                     bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

        else:
            x_range = bounds[0]
            y_range = bounds[1]
            resolution = 200

            x = np.linspace(x_range[0], x_range[1], resolution)
            y = np.linspace(y_range[0], y_range[1], resolution)
            X, Y = np.meshgrid(x, y)
            Z = np.zeros_like(X)

            for i in range(resolution):
                for j in range(resolution):
                    vec = [X[i, j], Y[i, j]]
                    try:
                        Z[i, j] = func(vec)
                    except:
                        Z[i, j] = np.nan

            contour = self.canvas.axes.contourf(X, Y, Z, levels=50, cmap='viridis')
            self.canvas.axes.contour(X, Y, Z, levels=10, colors='white', linewidths=0.5, alpha=0.5)

            self.canvas.axes.set_xlabel("x1", fontsize=12)
            self.canvas.axes.set_ylabel("x2", fontsize=12)
            self.canvas.axes.set_title(f"{func_name}", fontsize=14, fontweight='bold')

            if self.grid_checkbox.isChecked():
                self.canvas.axes.grid(True, alpha=0.3, linestyle='--')

            try:
                self.cbar = self.canvas.fig.colorbar(contour, ax=self.canvas.axes)
                self.cbar.set_label('f(x)', fontsize=10)
            except Exception as e:
                print(f"Warning: Could not create colorbar - {e}")

            info_text = ""
            if true_min_point is not None:
                true_x1, true_x2 = true_min_point
                true_z = func_info.get('true_value', func([true_x1, true_x2]))
                self.canvas.axes.plot(true_x1, true_x2, 'g*', markersize=12)
                info_text += f"Known minimum:\nf(x) = {true_z:.6f}\nx = ({true_x1:.6f}, {true_x2:.6f})\n"

            if min_point is not None:
                mx, my = min_point[0], min_point[1]
                mz = func(min_point)
                self.canvas.axes.plot(mx, my, 'r*', markersize=15)
                info_text += f"\nFound minimum:\nf(x) = {mz:.6f}\nx = ({mx:.6f}, {my:.6f})"

            if info_text:
                self.canvas.axes.text(0.02, 0.98, info_text, transform=self.canvas.axes.transAxes,
                                     verticalalignment='top', fontsize=9,
                                     bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

        self.canvas.draw()

    def run_optimization(self):
        func_name = self.func_combo.currentText()
        method_name = self.method_combo.currentText()

        func_info = AVAILABLE_FUNCTIONS[func_name]
        func = func_info["func"]
        bounds = func_info["bounds"]
        dim = func_info["dim"]

        count_points = get_count_points(dim)
        method = AVAILABLE_METHODS[method_name]

        try:
            if method_name == "Differential Evolution":
                options = global_opt_py.DEOptions()
                options.popsize = count_points
                result = method(func, bounds, options)
            else:
                result = method(func, bounds, count_point=count_points)

            if dim == 1:
                point_str = f"{result.x[0]:.6f}"
            else:
                point_str = f"({result.x[0]:.6f}, {result.x[1]:.6f})"

            # Known minimum information
            true_info = ""
            if "true_value" in func_info:
                true_info = f"\nKnown minimum: {func_info['true_value']:.6f}"
                if dim == 1:
                    true_info += f" at x={func_info['true_x']:.6f}"
                else:
                    true_x1, true_x2 = func_info['true_x']
                    true_info += f" at x=({true_x1:.6f}, {true_x2:.6f})"
                true_info += f"\nError: {abs(result.fun - func_info['true_value']):.10f}"

            # Update UI
            msg = (f"Function: {func_name}\n"
                   f"Method: {method_name}\n"
                   f"Points: {count_points}\n"
                   f"Found minimum: {result.fun:.6f}\n"
                   f"x={point_str}\n"
                   f"Evaluations: {result.nfev}"
                   f"{true_info}")
            self.result_label.setText(msg)

            # Redraw the plot
            self.plot_function(func_name, result.x)

        except Exception as e:
            self.result_label.setText(f"Error: {str(e)}")
            import traceback
            traceback.print_exc()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    window = OptimizerWindow()
    window.show()
    sys.exit(app.exec())