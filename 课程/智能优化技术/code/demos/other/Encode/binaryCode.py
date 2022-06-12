import numpy as np

class BinaryCode:

    def __init__(self, jump=0.1):
        if jump == 0:
            self.jump = 1
        else:
            self.jump = int(1/jump)
        # Vectorize functions
        self.dec2binv = np.vectorize(self.dec2bin)
        self.bin2decv = np.vectorize(self.bin2dec)

    def dec2bin(self, x):
        """
        Convert decimal to binary
        :param x: dedcimal number
        :return: binary number
        """
        x *= self.jump
        if x > 0:
            aux = '0'
        else:
            aux = '1'
        x = int(abs(x))
        return aux + bin(x)[2:]

    def bin2dec(self, x):
        """
        Binary to decimal
        :param x: Binary number
        :return: decimal number
        """
        aux = int(x[1:], 2)
        if x[0] == '1':
            aux = aux * -1
        return aux / self.jump
