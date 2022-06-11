function [result] = ComplexTrap( x_LowBound, x_Up_Bound,n)
% Inputs:
%        x_LowBound:积分区间下界
%        x_UpBound :积分区间上界
%        n         ：等分数量
% Outputs:
%        result    : 复化梯形积分结果

% 获取步长h
step_length = (x_Up_Bound - LowBound)/n;
%累积计算
result = 0;
for i = 1:n-1
    result = result + CalcuFunctionValue(x_LowBound+step_length*(i-1))+CalcuFunctionValue(x_LowBound+step_length*i);
end % 循环结束
result = result * step_length / 2;
end % 函数结束