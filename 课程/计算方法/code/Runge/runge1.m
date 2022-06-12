function runge1(n)
%n为Langrange差值节点的个数
x=linspace(-4,4,100);
plot(x,f(x),x,langrange(x,n));
end