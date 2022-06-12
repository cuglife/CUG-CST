import numpy as np
from other.Problems.interface_1D import Inerface_1D


class HardPolynomial(Inerface_1D):

    def __init__(self, jump=0.01, min_=-10, max_=10):
        f = lambda x: 30 * np.sin(x) - x ** 2 + 120
        super().__init__(jump=jump, min_=min_, max_=max_, f=f)
