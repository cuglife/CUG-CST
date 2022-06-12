# -*- coding: utf-8 -*-
"""
Created on Tue Dec  1 09:56:32 2020

@author: Amanda
"""

import random
import numpy as np
from numpy import linalg as la
import math
import colored
from termcolor import colored
from statistics import median
from optproblems import cec2005
import xlwt
from xlwt import Workbook

global MFES
global q_ant
global size_pop, MFES
global POP, POP_F
global min_values, max_values
global best_par
global gen_id
global num_iter
global etta

min_values = [-100, -100, -100, -100, -100, -100, -100, -100, -100, -100]
max_values = [100, 100, 100, 100, 100, 100, 100, 100, 100, 100]

MFES = 100000
q_ant = 0.00001
size_pop = 50
etta = 0.85

best_ref = [0 * MFES,
            0.001 * MFES,
            0.01 * MFES,
            0.1 * MFES,
            0.2 * MFES,
            0.3 * MFES,
            0.4 * MFES,
            0.5 * MFES,
            0.6 * MFES,
            0.7 * MFES,
            0.8 * MFES,
            0.9 * MFES,
            1.0 * MFES]


# POP_test = [[0,0],[1,2],[3,4],[5,8],[2,6]]


def Fitness(POP, num_iter):
    POP_F = []
    for particle in POP:
        evaluation1 = function1(particle)
        POP_F.append(evaluation1)
        num_iter += 1

        if num_iter > MFES:
            # print("Exceeded the limit of FES!!!")
            success = 0
            return
            break
    return POP_F, num_iter


# POP_test_F, num_iter = Fitness(POP_test, num_iter)

# print("Population: ", POP_test)
# print("Population fitness :", POP_test_F)
# print("Number of iterations :", num_iter)


def Initial_Pop():
    initial_pop = []
    for index1 in range(size_pop):
        variables = []

        for index2 in range(len(min_values)):
            variables.append(random.uniform(min_values[index2], max_values[index2]))

        initial_pop.append(variables)

    return initial_pop


def distance_manhattan(pa, pb):
    return la.norm(pa - pb, 1)


def Distance_norm(POP):
    distances = []
    for ant in POP:
        individual_distances = []
        for other_ant in POP:
            a = np.array(ant)
            b = np.array(other_ant)

            individual_distances.append(np.log1p(np.linalg.norm((a - b), ord=1)))

        distances.append(individual_distances)
    return distances


# print(Distance_norm(POP_test))


def Weigths(POP, POP_F):
    ant_fit_sort = sorted(POP_F)
    # print("Sorted fitnesses :", ant_fit_sort)
    ant_sort = []
    weigths = []

    for index1 in range(len(ant_fit_sort)):
        ant_sort.append(POP[POP_F.index(ant_fit_sort[index1])])

        weigth = (1 / (q_ant * size_pop * (math.sqrt(2 * math.pi)))) * math.exp(
            (-(index1 ** 2)) / (2 * (q_ant ** 2) * (size_pop ** 2)))

        weigths.append(weigth)

    return ant_sort, ant_fit_sort, weigths


def Prob_weigth(weigths):
    sum_weigths = np.sum(weigths)
    # print("Sum of weigths: ", sum_weigths)
    prob_weigths = []

    for index1 in range(len(weigths)):
        prob_weigths.append(weigths[index1] / sum_weigths)

    # print("Sum of probabilities", sum(prob_weigths))

    return prob_weigths


def Average_dist(distances):
    '''The average distance should be measured for each variable in the ant not the distance between the solutions
    -Testar implementar com um mínimo de desvio padrão'''
    average = []
    # print("Distances :", distances)
    for distance in distances:
        if distance != 0:
            average.append(distance)
    if len(average) == 0:
        return 0
    else:
        return np.mean(average)


def New_pop_1(POP, prob_weigths):
    new_pop = []

    distances = Distance_norm(POP)
    # print("Population: ", POP)
    # print("Distances: ", distances)
    # print("Probabilities: ", prob_weigths)

    for index1 in range(size_pop):
        new_solution = []

        selected = random.choices(POP, prob_weigths)
        selected_index = POP.index(selected[0])

        # print("Selected member :", selected)
        # print("Index of selected member :", selected_index)

        distance_index = distances[selected_index]

        average_dist = Average_dist(distance_index)

        # print("Selected member distances :", distance_index)
        # print("Average distance for selected member: ", average_dist)

        for index2 in range(len(selected[0])):

            rand_variable = random.gauss(selected[0][index2], average_dist * etta)

            if rand_variable > max_values[index2]:
                rand_variable = max_values[index2]

            elif rand_variable < min_values[index2]:
                rand_variable = min_values[index2]

            # print("Random variable generated: ", rand_variable)
            new_solution.append(rand_variable)

        new_pop.append(new_solution)
        # print("New current pop: ", new_pop)

    return new_pop


def Average(POP, selected_index, index2):
    selected = POP[selected_index]
    distances = []
    for member in POP:
        if member != selected:
            distances.append(abs(member[index2] - selected[index2]))

    # print("Distances :", distances)

    return np.average(distances)


def New_pop_2(POP, prob_weigths):
    new_pop = []

    for index1 in range(size_pop):
        new_solution = []

        selected = random.choices(POP, prob_weigths)
        selected_index = POP.index(selected[0])

        for index2 in range(len(selected[0])):

            average_dist = Average(POP, selected_index, index2)

            rand_variable = random.gauss(selected[0][index2], average_dist * etta)

            if rand_variable > max_values[index2]:
                rand_variable = max_values[index2]

            elif rand_variable < min_values[index2]:
                rand_variable = min_values[index2]

            # print("Random variable generated: ", rand_variable)
            new_solution.append(rand_variable)

        new_pop.append(new_solution)
        # print("New current pop: ", new_pop)

    return new_pop


def Selection_mode(ALL_POP, ALL_F):
    sorted_f = sorted(ALL_F)

    # print("Sorted fitnesses :", sorted_f)

    pop_sort = []

    for index1 in range(len(sorted_f)):
        pop_sort.append(ALL_POP[ALL_F.index(sorted_f[index1])])

    return pop_sort


def ACO():
    POP = Initial_Pop()
    print("Initial pop :", POP)

    gen_id = 0
    success = 0

    best_par = []

    current_parcial = 0

    num_iter = 0

    # POP_F, num_iter = Fitness(POP, num_iter)

    while num_iter < MFES:

        POP_F, num_iter = Fitness(POP, num_iter)

        print("Minimum solution founded: ", min(POP_F))
        print("Iteration number: ", num_iter)

        # print("POP fitness", POP_F)
        # print("Number of iterations :", num_iter)

        POP_sort, POP_F_sort, POP_weigths = Weigths(POP, POP_F)

        # print("Sorted pop : ", POP_sort)
        # print("Sorted fitness: ", POP_F_sort)
        # print("Population weigths: ", POP_weigths)

        POP_prob = Prob_weigth(POP_weigths)

        # print("Probability list: ", POP_prob)

        NEW_POP = New_pop_2(POP_sort, POP_prob)

        NEW_POP_F, num_iter = Fitness(NEW_POP, num_iter)

        # print("NEW_POP fitness", POP_F)
        # print("Number of iterations :", num_iter)

        POP = Selection_mode(POP + NEW_POP, POP_F + NEW_POP_F)[:size_pop]

        # POP_F, num_iter = Fitness(POP, num_iter)

        if num_iter >= best_ref[current_parcial]:
            best_par.append(min(POP_F) - global_min)
            current_parcial += 1
            print("Parcial comupted")

        if min(POP_F) - global_min < 0.00000001:
            best_in = min(POP_F) - global_min
            worst_in = max(POP_F) - global_min
            mean_in = np.mean(POP_F) - global_min
            median_in = np.median(POP_F)

            success = 1

            while len(best_par) < len(best_ref):
                best_par.append(best_in)

            print("Success")
            print("End in generation :", gen_id)
            print("End in Run nº: ", num_iter)
            print("Minimum result found in: ", POP[POP_F.index(min(POP_F))], " : ", best_in)
            return best_in, worst_in, mean_in, median_in, best_par, num_iter, success  # maximum_apt, average_apt, minimum_apt,
            break

        # print("The population after selection is: ", POP)

    print(POP_F)
    best_in = min(POP_F) - global_min
    worst_in = max(POP_F) - global_min
    mean_in = np.mean(POP_F) - global_min
    median_in = np.median(POP_F)

    success = 0

    while len(best_par) < len(best_ref):
        best_par.append(best_in)

    print("No Success")
    print("End in generation :", gen_id)
    print("End in Run nº: ", num_iter)
    print("Minimum result found in: ", POP[POP_F.index(min(POP_F))], " : ", best_in)
    return best_in, worst_in, mean_in, median_in, best_par, num_iter, success  # maximum_apt, average_apt, minimum_apt,


global_min = -450.0
function1 = cec2005.F4(10)


def Output_Excel(number_runs):
    success_rate = 0

    # Workbook is created
    wb = Workbook()

    # add_sheet is used to create sheet.
    sheet1 = wb.add_sheet('ACO')

    sheet1.write(1, 1, "RUN nº")
    sheet1.write(2, 1, "Closed in run")
    sheet1.write(3, 1, "Best result")
    sheet1.write(4, 1, "Worst result")
    sheet1.write(5, 1, "Mean result")
    sheet1.write(6, 1, "Median result")
    sheet1.write(7, 1, "Parcials")
    sheet1.write(8, 1, "Erro para FES=0,0*MaxFES")
    sheet1.write(9, 1, "Erro para FES=0,001*MaxFES")
    sheet1.write(10, 1, "Erro para FES=0,01*MaxFES")
    sheet1.write(11, 1, "Erro para FES=0,1*MaxFES")
    sheet1.write(12, 1, "Erro para FES=0,2*MaxFES")
    sheet1.write(13, 1, "Erro para FES=0,3*MaxFES")
    sheet1.write(14, 1, "Erro para FES=0,4*MaxFES")
    sheet1.write(15, 1, "Erro para FES=0,5*MaxFES")
    sheet1.write(16, 1, "Erro para FES=0,6*MaxFES")
    sheet1.write(17, 1, "Erro para FES=0,7*MaxFES")
    sheet1.write(18, 1, "Erro para FES=0,8*MaxFES")
    sheet1.write(19, 1, "Erro para FES=0,9*MaxFES")
    sheet1.write(20, 1, "Erro para FES=1,0*MaxFES")
    sheet1.write(21, 1, "Success rate")

    for run in range(number_runs):
        print("Start of run ", run)

        BEST, WORST, MEAN, MEDIAN, BEST_PAR, NUM_RUNS, SUCCESS = ACO()

        sheet1.write(1, run + 2, (run + 1))
        sheet1.write(2, run + 2, (NUM_RUNS))
        sheet1.write(3, run + 2, (BEST))
        sheet1.write(4, run + 2, (WORST))
        sheet1.write(5, run + 2, (MEAN))
        sheet1.write(6, run + 2, (MEDIAN))

        for index in range(len(BEST_PAR)):
            sheet1.write(8 + index, run + 2, (BEST_PAR[index]))

        success_rate += SUCCESS

    sheet1.write(21, 2, (success_rate))

    wb.save('./other/ACO.xls')

    return success_rate / number_runs


Output_Excel(3)
