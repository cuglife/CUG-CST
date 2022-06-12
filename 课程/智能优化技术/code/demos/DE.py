# -*- coding: utf-8 -*-
"""
Created on Wed Nov 11 10:29:11 2020

@author: Amanda
"""

from random import random
from random import sample
from random import uniform
import matplotlib.pyplot as plt
import pandas as pd


# --- FUNCTIONS ----------------------------------------------------------------+
def cost_func(x):
    global funEval
    from optproblems import cec2005

    problem = cec2005.F2(10)
    s = problem(x)
    funEval = funEval + 1
    return s


def ensure_bounds(vec, bounds):
    vec_new = []
    # cycle through each variable in vector 
    for i in range(len(vec)):

        # variable exceedes the minimum boundary
        if vec[i] < bounds[i][0]:
            vec_new.append(bounds[i][0])

        # variable exceedes the maximum boundary
        if vec[i] > bounds[i][1]:
            vec_new.append(bounds[i][1])

        # the variable is fine
        if bounds[i][0] <= vec[i] <= bounds[i][1]:
            vec_new.append(vec[i])

    return vec_new


# --- MAIN ---------------------------------------------------------------------+

def minimize(cost_func, bounds, popsize, mutate, recombination, maxiter):
    global funEval, SR
    maxFunEval = 1000000  # Maximum allowable function evaluations

    # --- INITIALIZE A POPULATION (step #1) ----------------+
    best_fitness = []

    population = []
    for i in range(0, popsize):
        indv = []
        for j in range(len(bounds)):
            indv.append(uniform(bounds[j][0], bounds[j][1]))
        population.append(indv)

    # --- SOLVE --------------------------------------------+

    # cycle through each generation (step #2)
    for i in range(1, maxiter + 1):
        print("GENERATION:", i)
        # print(population)
        gen_scores = []  # score keeping

        # cycle through each individual in the population
        for j in range(0, popsize):

            # --- MUTATION (step #3.A) ---------------------+

            # select three random vector index positions [0, popsize), not including current vector (j)
            candidates = list(range(0, popsize))
            candidates.remove(j)
            random_index = sample(candidates, 3)

            x_1 = population[random_index[0]]
            x_2 = population[random_index[1]]
            x_3 = population[random_index[2]]
            x_t = population[j]  # target individual

            # subtract x3 from x2, and create a new vector (x_diff)
            x_diff = [x_2_i - x_3_i for x_2_i, x_3_i in zip(x_2, x_3)]

            # multiply x_diff by the mutation factor (F) and add to x_1
            v_donor = [x_1_i + mutate * x_diff_i for x_1_i, x_diff_i in zip(x_1, x_diff)]
            v_donor = ensure_bounds(v_donor, bounds)

            # --- RECOMBINATION (step #3.B) ----------------+

            v_trial = []
            for k in range(len(x_t)):
                crossover = random()
                if crossover <= recombination:
                    v_trial.append(v_donor[k])

                else:
                    v_trial.append(x_t[k])

            # --- GREEDY SELECTION (step #3.C) -------------+

            score_trial = cost_func(v_trial)
            score_target = cost_func(x_t)

            if score_trial < score_target:
                population[j] = v_trial
                gen_scores.append(score_trial)
                # print( '   >',score_trial, v_trial)

            else:
                # print( '   >',score_target, x_t)
                gen_scores.append(score_target)

        # --- SCORE KEEPING --------------------------------+

        gen_avg = sum(gen_scores) / popsize  # current generation avg. fitness
        gen_best = min(gen_scores)  # fitness of best individual
        gen_sol = population[gen_scores.index(min(gen_scores))]  # solution of best individual

        print('      > GENERATION AVERAGE:', gen_avg)
        print('      > GENERATION BEST:', gen_best)
        print('         > BEST SOLUTION:', gen_sol, '\n')
        # best_fitness.append(gen_best)
        # plt.plot(best_fitness)
        # plt.ylabel('Aptidão')
        # plt.xlabel('Gerações')
        # plt.show()
        if funEval >= maxFunEval:
            break

    erro = gen_best - (-450)
    if erro <= 1e-08:
        SR = SR + 1
    pass


# --- CONSTANTS ----------------------------------------------------------------+
# D = 10
nRodadas = 2
df = pd.DataFrame(columns=['AG_01_10', 'Run1', 'Run2', 'Run3'])
SR = 0
bounds = [(-100, 100), (-100, 100), (-100, 100), (-100, 100), (-100, 100), (-100, 100), (-100, 100), (-100, 100),
          (-100, 100), (-100, 100)]  # bounds [(x1_min, x1_max), (x2_min, x2_max),...]
popsize = 25  # population size, must be >= 4
mutate = 0.9  # mutation factor [0,2]
recombination = 0.7  # recombination rate [0,1]
maxiter = 4000  # max number of generations

for rodada in range(1, nRodadas):
    funEval = 0
    minimize(cost_func, bounds, popsize, mutate, recombination, maxiter)
    print("Rodada: ", rodada)
    print(funEval)
