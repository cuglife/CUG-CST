import numpy as np
from matplotlib import pyplot as plt

X = np.array([0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8])
Y = np.array([0.6, 1.1, 1.6, 1.8, 2.0, 1.9, 1.7, 1.3])
m = []
for i in range(3):
    a = X ** i
    m.append(a)
A = np.array(m).T
b = Y.reshape(Y.shape[0], 1)


def projection(A, b):
    AA = A.T.dot(A)  # A乘以A转置
    w = np.linalg.inv(AA).dot(A.T).dot(b)
    print(w)
    return A.dot(w)


yw = projection(A, b)
yw.shape = (yw.shape[0],)
print(yw)
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
plt.title("This is Least squares fit.")
plt.xlabel("x")
plt.ylabel("y")
plt.scatter(X, Y, color='r', marker='.', label='拟合数据')
plt.plot(X, yw, color='b', label='拟合曲线')
plt.legend()
plt.show()
