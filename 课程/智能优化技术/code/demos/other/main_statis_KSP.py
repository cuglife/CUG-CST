import matplotlib.pyplot as plt
from Problems.knapsack import KSP
from Encode.binaryCodeGraph import BinaryCodeG

from GA import GeneticAlgorithm
from PBIL import PopulationBaseIncrementalLearning

from tqdm import tqdm
import numpy as np

size = 500

problem = KSP(size)

sol1 = []
sol2 = []
sol3 = []
sol4 = []
sol5 = []
sol6 = []
sol7 = []
sol8 = []
sol9 = []

for _ in tqdm(range(10)):
    solver1 = LocalSearch(problem)
    solver2 = SimmulatedAnneling(problem)
    solver3 = IteratedLocalSearch(problem, criterion='LSMC')
    solver4 = IteratedLocalSearch(problem, criterion='better')
    solver5 = IteratedLocalSearch(problem, criterion='rw')
    solver6 = GeneticAlgorithm(problem, rank=False, encode=BinaryCodeG(size))
    solver7 = GeneticAlgorithm(problem, rank=True, encode=BinaryCodeG(size))
    solver8 = PopulationBaseIncrementalLearning(problem, encode=BinaryCodeG(size))
    solver9 = TabuSearch(problem)
    solution1, _ = solver1()
    solution2, _ = solver2()
    solution3, _ = solver3()
    solution4, _ = solver4()
    solution5, _ = solver5()
    solution6, _ = solver6()
    solution7, _ = solver7()
    solution8, _ = solver8()
    solution9, _ = solver9()
    sol1.append(solution1)
    sol2.append(solution2)
    sol3.append(solution3)
    sol4.append(solution4)
    sol5.append(solution5)
    sol6.append(solution6)
    sol7.append(solution7)
    sol8.append(solution8)
    sol9.append(solution9)

sol1 = [problem(s, real=True) for s in sol1]
sol2 = [problem(s, real=True) for s in sol2]
sol3 = [problem(s, real=True) for s in sol3]
sol4 = [problem(s, real=True) for s in sol4]
sol5 = [problem(s, real=True) for s in sol5]
sol6 = [problem(s, real=True) for s in sol6]
sol7 = [problem(s, real=True) for s in sol7]
sol8 = [problem(s, real=True) for s in sol8]
sol9 = [problem(s, real=True) for s in sol9]

names = [solver1.abbreviation, solver2.abbreviation,
         solver3.abbreviation + '-L', solver4.abbreviation + '-B',
         solver5.abbreviation + '-R', solver6.abbreviation,
         solver7.abbreviation + '-r', solver8.abbreviation,
         solver9.abbreviation]

solution = [np.mean(sol1), np.mean(sol2),
            np.mean(sol3),
            np.mean(sol4), np.mean(sol5),
            np.mean(sol6), np.mean(sol7), np.mean(sol8),
            np.mean(sol9)]

plt.figure(figsize=(16,5))
plt.bar(names, solution)
plt.title("Performance")
plt.xlabel("Metaheuristics")
plt.ylabel("Cost Value")
plt.show()