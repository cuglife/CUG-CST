function H=heun(f,g,a,b,xa,ya,M)
%Input    - f is the function entered as a string 'f'
%         - g is the function entered as a string 'g'
%             - a and b are the left and right endpoints
%             - xa is the initial condition x(a)
%             - ya is the initial condition y(a)
%             - M is the number of steps
%Output - H=[T' X' Y'] where T is the vector of abscissas
%                and X and Y are the vectors of ordinates
h=(b-a)/M;
T=zeros(1,M+1);
X=zeros(1,M+1);
Y=zeros(1,M+1);
T=a:h:b;
X(1)=xa;
Y(1)=ya;
for j=1:M
    k1=feval(f,T(j),X(j),Y(j));
    k2=feval(g,T(j),X(j),Y(j));
    k3=feval(f,T(j+1),X(j)+h*k1,Y(j)+h*k2);
    k4=feval(g,T(j+1),X(j)+h*k1,Y(j)+h*k2);
    X(j+1)=X(j)+(h/2)*(k1+k3);
    Y(j+1)=Y(j)+(h/2)*(k2+k4);
end
H=[T' X' Y'];