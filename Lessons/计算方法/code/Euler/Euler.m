function E=Euler(f,a,b,ya,M)
 
%Input       - y'=f is the function 
%            - a and b are the left and right endpoints
%            - ya is the initial condition y(a)
%            - M is the number of steps
%Output - E=[T' Y'] where T is the vector of
 % abscissas and Y is the vector of ordinates

h=(b-a)/M;
T=zeros(1,M+1);
Y=zeros(1,M+1);
T=a:h:b;
Y(1)=ya;
 
for j=1:M
   Y(j+1)=Y(j)+h*feval(f,T(j),Y(j));
end
 
E=[T' Y'];