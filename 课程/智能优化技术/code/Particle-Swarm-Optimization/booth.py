from pso.pso import PSO
import numpy as np
import torch
import os

os.environ["KMP_DUPLICATE_LIB_OK"] = "TRUE"


def booth(x):
    return (x[0] + 2 * x[1] - 7) ** 2 + (2 * x[0] + x[1] - 5) ** 2


p = PSO(booth, "min", 20, 2, [[-10, 10], [-10, 10]], "cpu", True, True)

# print(p.solve(0.8, 0.5, 0.3))
print(p.solve(1000, 0.8, 0.4, 1.49445, 1.49445))
# print(p.solve(1000, 0.8, 0.4, 0.5, 0.3))