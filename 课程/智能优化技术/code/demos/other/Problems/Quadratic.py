from other.Problems.interface_1D import Inerface_1D


class Quadratic(Inerface_1D):

    def __init__(self, jump=0.01, min_=-1, max_=3):
        f = lambda x: x**4 - 5*x**3 + 5 * x**2 + 5 * x + 0.91
        super().__init__(jump=jump, min_=min_, max_=max_, f=f)
