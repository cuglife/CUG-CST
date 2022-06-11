function [R,k,T]=romberg(fun,a,b,tol)
% 龙贝格(Romberg数值求解公式)
% author:
%   -gongwanlu
% inputs:
%   -fun：积分函数句柄
%   -a/b：积分上下限
%   -tol：积分误差
% Outputs:
%   -R：7阶精度Romberg积分值
%   -k：迭代次数
%   -T：整个迭代过程
%
% Example
% fun=@(x)4./(1+x^2);
% [R,k,T]=romberg(fun,0,1,1e-6)
%
k=0; % 迭代次数
n=1; % 区间划分个数
h=b-a;
T=h/2*(fun(a)+fun(b));
err=1;
while err>=tol
    k=k+1;
    h=h/2;
    tmp=0;
    for i=1:n
        tmp=tmp+fun(a+(2*i-1)*h);
    end
    T(k+1,1)=T(k)/2+h*tmp;
    for j=1:k
        T(k+1,j+1)=T(k+1,j)+(T(k+1,j)-T(k,j))/(4^j-1);
    end
    n=n*2;
    err=abs(T(k+1,k+1)-T(k,k));
end
R=T(k+1,4);