# 导入第三方包
import copy

import matplotlib.pyplot as plt
import numpy as np

from other.Encode.binaryCode import BinaryCode
from other.Problems.Parabola import Parabola


class GeneticAlgorithm:
    def __init__(self, problem, elit_=True, rank=False,
                 encode=None):
        """
        Initialize the algorithm class 初始化算法类
        """
        self.problem = problem  # 解决的问题
        if encode is None:  # 编码转换，默认转换为二进制
            self.bc = BinaryCode(jump=problem.jump)
        else:
            self.bc = encode
        self.name = "Genetic Algorithm"  # 算法名称
        self.abbreviation = 'GA'
        self.do_rank = rank  # 是否排序
        if elit_:            # 是否使用精英主义重建种群
            self.reconstruction = self.elit
        else:
            self.reconstruction = self.noElite

    def generatePopulation(self, population_size):
        """
        Generate a new population                       种群初始化
        :param population_size: Size of the population  种群大小
        :return: generated population encoded           生成的种群编码
        """
        return self.bc.dec2binv([self.problem.give_random_point()
                                 for _ in range(population_size)])

    def crossover(self, couples, population_size):
        """
        Crossover of couples                     单点杂交
        :param couples: couples list             父代列表
        :param population_size: population size  群体大小
        :return: childs generated                生成的子代
        """
        childs = []  # 初始化子代
        for i, j in couples:  # 在父代中选择两个进行杂交
            mid = np.random.choice(np.arange(len(i)))  # 随机选择交换点
            childs.append(i[:mid] + j[mid:])  # 子代1
            childs.append(j[:mid] + i[mid:])  # 子代2
        return childs[:population_size]

    def rank(self, fitness):
        """
        Rank the fitness                    对适应度进行排序
        :param fitness: fitness values      适应值
        :return: fitness updated            归一化后的适应值
        """
        Pop_worst = np.min(fitness)  # 种群中最小适应值
        Pop_best = np.max(fitness)   # 种群中最大适应值
        N = len(fitness)             # 种群数量
        aux = np.zeros(N)
        for i in range(aux.shape[0]):
            aux[i] = (1 / N) * (Pop_worst + (Pop_best - Pop_worst) * ((fitness[i] - 1) / (N - 1)))

        return aux

    def selection(self, parents_num, eval, population):
        """
        Selection parents 轮盘赌选择父代
        :param parents_num: number of parents 父代数量
        :param eval: evaluation population    种群评价值
        :param population: population list    种群
        :return: parent list                  筛选出的父代
        """
        parents = []             # 初始化
        if self.do_rank:         # 如果已排序则使用排序后的适应值
            aux = self.rank(eval)
        aux = np.max(eval) - np.array(eval)  # 适应值最大值减去每个适应值
        prob = aux / np.sum(aux)             # 计算概率
        for _ in range(parents_num):         # 按适应值大小随机选择父代
            parents.append(np.random.choice(population, p=prob, size=2))
        return parents

    def mutation(self, population, mutation=0.01):
        """
        Generate mutations in the population      在种群中产生突变
        :param population: list of the population 种群链表
        :param mutation: mutation rate            变异率
        :return: mutated population               变异后的种群
        """
        new_population = []   # 新的种群
        for p in population:  # 对于种群中每个个体
            n = len(p)
            d = np.random.choice([0, 1], p=[1 - mutation, mutation], size=n)  # 随机选择是否突变 1表示突变 0 代表不突变
            aux = copy.copy(p)
            for j, di in enumerate(d):
                if di:                              # 如果该位突变，则将该位做非运算
                    aux = list(aux)
                    aux[j] = str(int(not int(aux[j])))
                    aux = "".join(aux)
            new_population.append(copy.copy(aux))   # 将变异后的个体加入种群
        return new_population

    def noElite(self, population, childs):
        """
        Reconstruction with no elitism          不使用精英主义重建
        :param population: list population      种群链表
        :param childs: list childs              子代链表
        :return: new population                 新种群
        """
        return childs  # 直接返回子代(舍弃父代

    def elit(self, population, childs):
        """
        Reconstruction with elitism        使用精英主义重建
        :param population: list population 种群链表
        :param childs: list childs         子代链表
        :return: new population            新种群
        """
        total = list(childs) + list(population)         # 种群总数量 = 父代 + 子代
        eval = self.problem(self.bc.bin2decv(total))    # 种群适应值
        total_eval = list(zip(total, eval))             # zip 为字典
        total_eval.sort(key=lambda x: x[1])             # 以适应值排序
        total, _ = zip(*total_eval)                     # 解压
        total = list(total)                             # 组成链表
        return total[:len(population)]                  # 将原种群数目后适应值差的个体淘汰

    def __call__(self, max_iter=1000, population_size=6, parents_num=4):
        """
        Solver 问题求解
        :param max_iter: max number of iterations  最大迭代次数
        :param population_size: Size population    种群大小
        :param parents_num: Number of parents      父代数量
        :return: Solution and history of solutions 算法解
        """
        population = self.generatePopulation(population_size)   # 群体初始化
        eval = self.problem(self.bc.bin2decv(population))       # 获取群体适应值
        history = [population[np.argmin(eval)]]                 # 存储每代种群
        for _ in range(max_iter):
            parents = self.selection(parents_num, eval, population)  # 轮盘赌选择父代
            childs = self.crossover(parents, population_size)        # 单点杂交
            childs = self.mutation(childs)                           # 变异
            population = self.reconstruction(population, childs)     # 子代与父代重建新种群
            eval = self.problem(self.bc.bin2decv(population))        # 更新适应值
            history.append(population[np.argmin(eval)])              # 记录历史种群
            if np.sum(np.max(eval) - np.array(eval)) == 0:           # 如果适应值相同，退出
                break
        solution = self.bc.bin2dec(population[np.argmin(eval)])      # 问题解转换为十进制
        history = self.bc.bin2decv(history)                          # 历史解转换为十进制
        return solution, history


problem = Parabola()             # 求解问题
x, y = problem.generate_space()  # 求解范围

solver = GeneticAlgorithm(problem, rank=False)  # 初始化遗传算法类
solution, iterations = solver(max_iter=400)     # 生成算法解

# 绘图
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
