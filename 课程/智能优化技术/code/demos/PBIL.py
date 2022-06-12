from other.Encode.binaryCode import BinaryCode
import numpy as np


class PopulationBaseIncrementalLearning:

    def __init__(self, problem, encode=None):
        self.problem = problem
        if encode is None:
            self.bc = BinaryCode(jump=problem.jump)
            self.size_given = False
        else:
            self.bc = encode
            self.size_given = True
        self.name = "Population Base Incremental Learning"
        self.abbreviation = 'PBIL'

    def generate(self, P, generation_number):
        """
        Generate population form Array P
        :param P: array P
        :param generation_number: Population size generated
        :return: Population generated
        """
        population = []
        for _ in range(generation_number):
            aux = [str(np.random.choice([0, 1], p=[1 - p, p]))
                   for p in P]
            aux = "".join(aux)
            if self.size_given:
                while not (self.problem.min_ <=
                           self.problem.isValid(self.bc.bin2dec(aux))
                           <= self.problem.max_):
                    aux = [str(np.random.choice([0, 1], p=[1 - p, p]))
                           for p in P]
                    aux = "".join(aux)
            else:
                while not (self.problem.min_ <= self.bc.bin2decv(aux) <= self.problem.max_):
                    aux = [str(np.random.choice([0, 1], p=[1 - p, p]))
                           for p in P]
                    aux = "".join(aux)
            population.append(aux)
        return population

    def __call__(self, max_iter=1000, learning_rate=0.01,
                 generation_number=5, number_update=2):
        """
         Solver
         :param max_iter: max number of iterations
         :param learning_rate: learning rate
         :param generation_number: Population size generated
         :param number_update: population use to update P
         :return: Solution and history of solutions
        """
        # Initialize P
        if self.size_given:
            P = np.ones(self.bc.num_nodes) * 0.5
        else:
            max_value = (1 / self.problem.jump) * (self.problem.max_ -
                                                   self.problem.min_)

            P = np.ones(len(self.bc.dec2bin(max_value))) * 0.5
        population = self.generate(P, generation_number)
        eval = self.problem(self.bc.bin2decv(population))
        history = [population[np.argmin(eval)]]
        for _ in range(max_iter):
            pop_eva = list(zip(population, eval))
            pop_eva.sort(key=lambda x: x[1])
            population, _ = zip(*pop_eva)
            for i in range(number_update):
                aux = np.array([int(i) for i in list(population[i])])
                P = (1 - learning_rate) * P + learning_rate * aux
            population = self.generate(P, generation_number)
            eval = self.problem(self.bc.bin2decv(population))
            history.append(population[np.argmin(eval)])
        solution = self.bc.bin2dec(population[np.argmin(eval)])
        history = self.bc.bin2decv(history)
        return solution, history


import matplotlib.pyplot as plt
from other.Problems.hardPolynomial import HardPolynomial

problem = HardPolynomial()
x, y = problem.generate_space()

solver = PopulationBaseIncrementalLearning(problem)
solution, iterations = solver(max_iter=200)

plt.figure(figsize=(20, 5))
plt.subplot(1, 2, 1)
plt.title(solver.name)
plt.plot(x, y, label='Problem Space')
plt.plot(iterations, problem(iterations), '-o', label='Solutions')
plt.plot([solution], problem([solution]), 'o', label='Final Solution')
plt.legend()
plt.subplot(1, 2, 2)
plt.title("Cost Evolution")
plt.plot(problem(iterations))
plt.xlabel("Iterations")
plt.ylabel("Cost value")
plt.show()
