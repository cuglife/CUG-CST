import matplotlib.pyplot as plt
from Problems.hardPolynomial import HardPolynomial

from GA import GeneticAlgorithm
from PBIL import PopulationBaseIncrementalLearning

from tqdm import tqdm
import numpy as np



problem = HardPolynomial()
x, y = problem.generate_space()


sol1 = []
sol2 = []
sol3 = []
sol4 = []
sol5 = []
sol6 = []
sol7 = []
sol8 = []
sol9 = []
for _ in tqdm(range(100)):
    solver1 = LocalSearch(problem)
    solver2 = SimmulatedAnneling(problem)
    solver3 = IteratedLocalSearch(problem, criterion='LSMC')
    solver4 = IteratedLocalSearch(problem, criterion='better')
    solver5 = IteratedLocalSearch(problem, criterion='rw')
    solver6 = GeneticAlgorithm(problem, rank=False)
    solver7 = GeneticAlgorithm(problem, rank=True)
    solver8 = PopulationBaseIncrementalLearning(problem)
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

names = [solver1.abbreviation, solver2.abbreviation,
         solver3.abbreviation + '-L', solver4.abbreviation + '-B',
         solver5.abbreviation + '-R', solver6.abbreviation,
         solver7.abbreviation + '-r', solver8.abbreviation,
         solver9.abbreviation]
solution = [np.mean(problem(sol1)), np.mean(problem(sol2)),
            np.mean(problem(sol3)),
            np.mean(problem(sol4)), np.mean(problem(sol5)),
            np.mean(problem(sol6)), np.mean(problem(sol7)),
            np.mean(problem(sol8)), np.mean(problem(sol9))]

plt.figure(figsize=(16,5))
plt.bar(names, solution)
plt.title("Performance")
plt.xlabel("Metaheuristics")
plt.ylabel("Cost Value")
plt.show()