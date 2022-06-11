function Cn = Cn(a,b,n)
    format long
    h = (b-a)/n;
    sum1 = 0;
    sum2 = 0;
    for i = 0:n-1
        sum1 = sum1 + 32*f(a+(i+1/4).*h)+12*f(a+(i+1/2).*h)+32*f(a+(i+3/4).*h);
    end
    for j = 1:n-1
        sum2 = sum2 + 14*f(a+j.*h);
    end
Cn = h/90*(7*f(a)+sum1+sum2+7*f(b));