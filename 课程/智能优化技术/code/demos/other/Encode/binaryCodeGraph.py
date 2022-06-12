import numpy as np


class BinaryCodeG:

    def __init__(self, jump):
        self.num_nodes = jump

    def dec2bin(self, x):
        aux = np.zeros(self.num_nodes)
        for xi in x:
            aux[xi] = 1
        return "".join(aux.astype(int).astype(str))

    def bin2dec(self, x):
        x = np.array(list(x)).astype(int)
        return list(np.nonzero(x)[0])

    def dec2binv(self, x_list):
        aux = []
        for x in x_list:
            aux.append(self.dec2bin(x))
        return aux

    def bin2decv(self, x_list):
        aux = []
        for x in x_list:
            aux.append(self.bin2dec(x))
        return aux



