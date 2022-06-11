function R=rk4(f,g,a,b,xa,ya,M)
%Input    - f is the function entered as a string 'f'
%         - g is the function entered as a string 'g'
%             - a and b are the left and right endpoints
%             - xa is the initial condition x(a)
%             - ya is the initial condition y(a)
%             - M is the number of steps
%Output -R=[T' X' Y'] where T is the vector of abscissas
%              and X and Y are the vectors of ordinates
h=(b-a)/M;
T=zeros(1,M+1);
X=zeros(1,M+1);
Y=zeros(1,M+1);
T=a:h:b;
X(1)=xa;
Y(1)=ya;
for j=1:M
    k1=h*feval(f,T(j),X(j),Y(j));
    k2=h*feval(g,T(j),X(j),Y(j));
    k3=h*feval(f,T(j)+h/2,X(j)+k1/2,Y(j)+k2/2);
    k4=h*feval(g,T(j)+h/2,X(j)+k1/2,Y(j)+k2/2);
    k5=h*feval(f,T(j)+h/2,X(j)+k3/2,Y(j)+k4/2);
    k6=h*feval(g,T(j)+h/2,X(j)+k3/2,Y(j)+k4/2); 
    k7=h*feval(f,T(j)+h,X(j)+k5,Y(j)+k6);
    k8=h*feval(g,T(j)+h,X(j)+k5,Y(j)+k6);
    X(j+1)=X(j)+(k1+2*k3+2*k5+k7)/6;
    Y(j+1)=Y(j)+(k2+2*k4+2*k6+k8)/6;
end
R=[T' X' Y'];