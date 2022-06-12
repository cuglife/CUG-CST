# -*- coding: utf-8 -*-
"""
Created on Thu Nov 12 11:53:32 2020

@author: Amanda
"""

import numpy  as np
import math
import random
import os
import pandas as pd


# --- FUNCTIONS ----------------------------------------------------------------+
def cost_func(x):
    global funEval
    from optproblems import cec2005

    problem = cec2005.F1(D)
    s = problem(x)
    funEval = funEval + 1
    return s


# Function: Initialize Variables
def initial_position(swarm_size, min_values, max_values, cost_func):
    position = np.zeros((swarm_size, len(min_values) + 1))
    for i in range(0, swarm_size):
        for j in range(0, len(min_values)):
            position[i, j] = random.uniform(min_values[j], max_values[j])
        position[i, -1] = cost_func(position[i, 0:position.shape[1] - 1])
    return position


# Function: Initialize Velocity
def initial_velocity(position, min_values, max_values):
    init_velocity = np.zeros((position.shape[0], len(min_values)))
    for i in range(0, init_velocity.shape[0]):
        for j in range(0, init_velocity.shape[1]):
            init_velocity[i, j] = random.uniform(min_values[j], max_values[j])
    return init_velocity


# Function: Individual Best
def individual_best_matrix(position, i_b_matrix):
    for i in range(0, position.shape[0]):
        if (i_b_matrix[i, -1] > position[i, -1]):
            for j in range(0, position.shape[1]):
                i_b_matrix[i, j] = position[i, j]
    return i_b_matrix


# Function: Velocity
def velocity_vector(position, init_velocity, i_b_matrix, best_global, w, c1, c2):
    r1 = int.from_bytes(os.urandom(8), byteorder="big") / ((1 << 64) - 1)
    r2 = int.from_bytes(os.urandom(8), byteorder="big") / ((1 << 64) - 1)
    velocity = np.zeros((position.shape[0], init_velocity.shape[1]))
    for i in range(0, init_velocity.shape[0]):
        for j in range(0, init_velocity.shape[1]):
            velocity[i, j] = w * init_velocity[i, j] + c1 * r1 * (i_b_matrix[i, j] - position[i, j]) + c2 * r2 * (
                        best_global[j] - position[i, j])
    return velocity


# Function: Updtade Position
def update_position(position, velocity, min_values, max_values, cost_func):
    for i in range(0, position.shape[0]):
        for j in range(0, position.shape[1] - 1):
            position[i, j] = np.clip((position[i, j] + velocity[i, j]), min_values[j], max_values[j])
        position[i, -1] = cost_func(position[i, 0:position.shape[1] - 1])
    return position


nRodadas = 2
df = pd.DataFrame(
    columns=['AG_01_10', 'Run1', 'Run2', 'Run3', 'Run4', 'Run5', 'Run6', 'Run7', 'Run8', 'Run9', 'Run10', 'Run11',
             'Run12', 'Run13', 'Run14', 'Run15', 'Run16', 'Run17', 'Run18', 'Run19', 'Run20', 'Run21', 'Run22', 'Run23',
             'Run24', 'Run25'])
SR = 0
D = 10
swarm_size = 1000
min_values = D * [-100]
max_values = D * [100]
iterations = 1000
decay = 0
w = 0.6571
c1 = 1.6319
c2 = 0.6239

maxiter = 2000  # max number of generations
for rodada in range(1, nRodadas):

    funEval = 0
    maxFunEval = 100000  # Maximum allowable function evaluations
    position = initial_position(swarm_size, min_values, max_values, cost_func)
    init_velocity = initial_velocity(position, min_values=min_values, max_values=max_values)
    i_b_matrix = np.copy(position)
    best_global = np.copy(position[position[:, -1].argsort()][0, :])
    for i in range(1, maxiter + 1):
        print("Iteration = ", i, " f(x) = ", best_global[-1])
        position = update_position(position, init_velocity, min_values, max_values, cost_func)
        i_b_matrix = individual_best_matrix(position, i_b_matrix)
        value = np.copy(i_b_matrix[i_b_matrix[:, -1].argsort()][0, :])
        if (best_global[-1] > value[-1]):
            best_global = np.copy(value)
            # if (decay > 0):
        #     n  = decay
        #     w  = w*(1 - ((i-1)**n)/(iterations**n))
        #     c1 = (1-c1)*(i/iterations) + c1
        #     c2 = (1-c2)*(i/iterations) + c2
        init_velocity = velocity_vector(position, init_velocity, i_b_matrix, best_global, w=w, c1=c1, c2=c2)
        if funEval >= maxFunEval:
            break
    erro = best_global[-1] - (-450)
    if erro <= 1e-08:
        SR = SR + 1
    pass
    print("Rodada: ", rodada)
    print(funEval)
    print(best_global[-1])

df.to_csv(r'D:\repos\opt\other\PSO.csv', index=False)
