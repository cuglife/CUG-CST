from other.Problems.interface_1D import Inerface_1D


class Parabola(Inerface_1D):

    def __init__(self, jump=0.1, min_=-5, max_=5):
        f = lambda x: x ** 2
        super().__init__(jump=jump, min_=min_, max_=max_, f=f)