import numpy as np

from functions.function import Function


class Rosen(Function):
    def __init__(self):
        super().__init__()
        self.NUMM_HILL_COEFF = 14
        self.aHill = [0.980773, -0.794122, -0.203711, 0.394696, -0.0102847, 0.53679, -0.609485, 0.870846, 0.726005,
                      0.00979644, -0.601245, -0.853633, 0.25248, 0.608936]
        self.bHill = [-0.876339, -0.629688, 0.695303, 0.00503555, 0.783258, -0.358318, 0.541063, -0.742302, 0.346843,
                      -0.088168, 0.0708335, -0.0198675, -0.75164, 0.989074]
        self.minBound = [0.0]
        self.maxBound = [1.0]

    def compute(self, x):
        """The Rosenbrock function - works with regular Python lists/tuples"""
        result = 0.0
        for i in range(len(x) - 1):
            result += 100.0 * (x[i + 1] - x[i] ** 2.0) ** 2.0 + (1 - x[i]) ** 2.0
        return result

    def get_low_bound(self):
        return -1

    def get_up_bound(self):
        return 11

    def get_dim(self):
        return 2
