function [result] = ComplexSimpson(x_LowBound, x_Up_Bound,n)
% simpson求积公式
% Inputs:
%        x_LowBound:积分区间下界
%        x_UpBound :积分区间上界
%        n         ：等分数量,需要为2n等分，即节点个数必须满足2n+1
% Outputs:
%        result    : 复化Simpson积分结果

% 判断积分区间个数是否是2的倍数，满足则进行计算，否则打印提示
if mod(n,2) == 0
    % 获取步长h
    step_length = (x_Up_Bound - LowBound)/n;
    %累积计算
    result = 0;
    for i = 1:2:n-1
        result = result + CalcuFunctionValue(x_LowBound+step_length*(i-1))...
                 +4*CalcuFunctionValue(x_LowBound+step_length*i)...
                 +CalcuFunctionValue(x_LowBound+step_length*(i+1));
    end % 循环结束
    result = result*step_length/6;
else
??? print('等分区间数错误！');
end % if判断结束
end % 函数结束