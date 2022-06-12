%计算x对应的函数值f(x)
function [y_x] = CalcuFunctionValue(x)
% inputs:
%        x:待求值
% outputs:
%        y_x:x对应的函数值

% 根据极限计算，当x→0时，y_x→1.
if x == 0
    y_x = 1;
else
    y_x = sin(x)/x;
end
end