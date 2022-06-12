from abc import ABC, abstractmethod

class Problem_Interface(ABC):

    @abstractmethod
    def give_random_point(self):
        pass

    @abstractmethod
    def generate_space(self):
        pass

    @abstractmethod
    def generate_neighbour(self, x):
        pass

    @abstractmethod
    def all_neighbours(self, x):
        pass

    @abstractmethod
    def perturbation(self, solution, p=4):
        pass

    @abstractmethod
    def __call__(self, x):
        pass