%计算（4/1+X^2）在0到1上面的积分
a = 0
b = 1
epsilon = 5e-6
f = @(x)4 ./ (1 + x^2);
y = romberg(f,a,b,epsilon) ;