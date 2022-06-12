import torch
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


class PSO:
    """
    Particle Swarm optimization class
    """

    def __init__(self, opt_func, opt_task, num_particles, num_dims, constraints, device, plot=False, animate=False):
        """
        Constructor for PSO class

        Inputs:
        param opt_func : 传递要优化的函数对象
        param opt_task : 选择 "min" or "max"
        param num_particles: 指定粒子群优化中的粒子数
        param num_dims: 维度
        param constraints: 搜索空间，约束
                Example  : [[-10,10],[-15,15]] where -10 is min for first indp. var., 10 is max for first indp. var. and so on.
        param device    : cpu or gpu ("cuda")
        param plot      : 是否绘制最佳拟合vs迭代图。
        param animate   : 是否生成动画
        """
        # 判断gpu是否可用
        if device == "gpu":
            device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
        self.device = device
        print(f"Using {device}")

        self.opt_func = opt_func
        self.opt_task = opt_task
        self.num_particles = num_particles
        self.num_dims = num_dims
        self.constraints = torch.Tensor(constraints).T.to(device)
        self.particle_pos = self.constraints[0, :] + torch.rand(num_particles, num_dims).to(device) * (
                self.constraints[1, :] - self.constraints[0, :])
        self.particle_vels = torch.randn(num_particles, num_dims).to(device)
        self.particle_pbests = self.particle_pos
        self.gbest = torch.randn(num_dims).to(device)
        self.particle_fitness = torch.randn(num_particles).to(device)
        self.particle_best_fitness = self.particle_fitness
        self.best_gbest_fitness = 2
        self.plot = plot
        self.animate = animate
        if self.plot:
            self.gbest_fitness_lst = []
        if self.animate:
            self.best_particles_lst = []

    def _update_anim(self, index, scat, ax):
        """
        用于创建动画的更新功能。

        Input:
        param  index: 帧数
        param  scat : 散点图对象以更新粒子位置
        param  ax   : 更新坐标标题
        """
        scat.set_offsets(([self.best_particles_lst[index][0], self.best_particles_lst[index][1]]))
        ax.set_title(f"Iteration {index}")

    def _animate(self, num_iterations):
        """
        创建并保存 最佳粒子动画

        Inputs:
        param num_iterations：PSO收敛的迭代次数
        """
        x = np.linspace(self.constraints[0, 0], self.constraints[1, 0], 100)
        y = np.linspace(self.constraints[0, 1], self.constraints[1, 1], 100)
        X, Y = np.meshgrid(x, y)
        Z = self.opt_func([torch.Tensor(X), torch.Tensor(Y)])
        fig = plt.figure()
        ax = plt.axes(xlim=(self.constraints[0, 0], self.constraints[1, 0]),
                      ylim=(self.constraints[0, 1], self.constraints[1, 1]))
        ax.contour(X, Y, Z)
        scat = ax.scatter(self.best_particles_lst[0][0], self.best_particles_lst[0][1], c="r")
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_title("Iteration 0")
        anim = FuncAnimation(fig, self._update_anim, fargs=(scat, ax), interval=100)
        anim.save("new_video.gif")

    def _get_fitness(self):
        """
        计算所有PSO粒子的适应度值
        """
        self.particle_fitness = self.opt_func([self.particle_pos[:, dim] for dim in range(self.num_dims)])

    def _update_bests(self, iteration):
        """
        更新所有粒子的pbest和gbest。

        Inputs:
        param iteration：当前迭代次数
        """
        if iteration == 1:
            self.particle_best_fitness = self.particle_fitness
            self.particle_pbests = self.particle_pos
            if self.opt_task == "max":
                fitness_val, index = self.particle_best_fitness.max(0)
            else:
                fitness_val, index = self.particle_best_fitness.min(0)
            if self.animate:
                self.best_particles_lst.append(self.particle_pos[index])
            self.best_gbest_fitness = fitness_val
            self.gbest = self.particle_pbests[index]
        else:
            if self.opt_task == "max":
                mask1 = (self.particle_best_fitness > self.particle_fitness).type(torch.int)
                mask2 = (self.particle_best_fitness < self.particle_fitness).type(torch.int)
                self.particle_best_fitness = mask1 * self.particle_best_fitness + mask2 * self.particle_fitness
                self.particle_pbests = mask1.reshape((self.num_particles, 1)) * self.particle_pbests + mask2.reshape(
                    (self.num_particles, 1)) * self.particle_pos
                fitness_val, index = self.particle_best_fitness.max(0)
                if self.animate:
                    self.best_particles_lst.append(self.particle_pos[index])
                if fitness_val > self.best_gbest_fitness:
                    self.best_gbest_fitness = fitness_val
                    self.gbest = self.particle_pbests[index]
            else:
                mask1 = (self.particle_best_fitness < self.particle_fitness).type(torch.int)
                mask2 = (self.particle_best_fitness > self.particle_fitness).type(torch.int)
                self.particle_best_fitness = mask1 * self.particle_best_fitness + mask2 * self.particle_fitness
                self.particle_pbests = mask1.reshape((self.num_particles, 1)) * self.particle_pbests + mask2.reshape(
                    (self.num_particles, 1)) * self.particle_pos
                fitness_val, index = self.particle_best_fitness.min(0)
                if self.animate:
                    self.best_particles_lst.append(self.particle_pos[index])
                if fitness_val < self.best_gbest_fitness:
                    self.best_gbest_fitness = fitness_val
                    self.gbest = self.particle_pbests[index]

    def solve(self, max_iteration, w_max, w_min, c1, c2):
        """
        在约束内对 opt_func 运行 PSO 以执行 opt_task
        Inputs:
        param  w : 粒子速度权重
        param c1 : 粒子向个体最佳方向 pbest 移动速度权重
        param c2 : 粒子向群体最佳方向 gbest 移动速度权重

        Outputs:
        2-tuple :（最佳粒子的位置，最佳粒子的评估函数值）
        """
        iteration = 1
        while iteration <= max_iteration and (not (self.particle_pos[0] == self.particle_pos).all().item()):
            self._get_fitness()
            self._update_bests(iteration)
            # Updating velocities of particles
            w = w_min + float(iteration / max_iteration) * (w_max - w_min)
            self.particle_vels = w * self.particle_vels \
                                 + c1 * torch.rand(1).to(self.device) * (self.particle_pbests - self.particle_pos) \
                                 + c2 * torch.rand(1).to(self.device) * (self.gbest - self.particle_pos)

            # 更新粒子的位置
            self.particle_pos = self.particle_pos + self.particle_vels

            # 粒子离开受限搜索空间时的约束位置
            for dim in range(self.num_dims):
                torch.clamp_(self.particle_pos[:, dim], min=self.constraints[0, dim], max=self.constraints[1, dim])

            # 绘图
            if self.plot:
                self.gbest_fitness_lst.append(self.best_gbest_fitness)

            # 输出迭代值
            print(f"Iteration {iteration} : Gbest = {self.gbest}, Gbest_fitness : {self.best_gbest_fitness}")
            print("__________________________________________________________________________________")
            iteration += 1

        # 绘制最佳适应度与迭代次数
        if self.plot:
            plt.plot(range(iteration - 1), self.gbest_fitness_lst)
            plt.xlabel("Iteration Number")
            plt.ylabel("Fitness Value")
            plt.xscale("log")
            plt.savefig("new_plot.png")

        # 设置最佳粒子的动画
        if self.animate:
            self._animate(iteration - 1)

        return self.gbest, self.opt_func([self.gbest[dim] for dim in range(self.num_dims)])
