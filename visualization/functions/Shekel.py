import numpy as np

from functions.function import Function


class Shekel(Function):
    def __init__(self):
        super().__init__()
        self.NUMM_HILL_COEFF = 10
        self.kShekel = [23.93246, 17.63833, 18.27250, 19.15204, 14.05667, 9.04614, 17.96243, 20.56200, 21.70278,
                        10.51653]
        self.aShekel = [6.96615, 4.56374, 3.80749, 7.42302, 5.16861, 3.25999, 8.68618, 7.24754, 0.22309, 0.78036]
        self.cShekel = [1.11196, 1.11025, 1.19145, 1.13756, 1.17645, 1.18894, 1.18148, 1.16138, 1.10642, 1.04509]
        self.low_bound = 0.0
        self.up_bound = 10.0

    def compute(self, input):
        x = np.asarray(input)

        res = np.zeros_like(x, dtype=float)

        for i in range(self.NUMM_HILL_COEFF):
            denominator = self.kShekel[i] * np.power(x - self.aShekel[i], 2) + self.cShekel[i]
            res = res - 1.0 / denominator

        return res

    def get_low_bound(self):
        return self.low_bound

    def get_up_bound(self):
        return self.up_bound

    def get_dim(self):
        return 1