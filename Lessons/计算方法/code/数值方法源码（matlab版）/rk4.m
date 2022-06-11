function R=rk4(f,a,b,ya,M)

%Input    - f is the function 
%            - a and b are the left and right endpoints
%            - ya is the initial condition y(a)
%            - M is the number of steps
%Output - R = [T' Y'] where T is the vector of abscissas
%              and Y is the vector of ordinates

%If f is an M-file function call R=rk4(@f,a,b,ya,M).
%If f is an anonymous function call R=rk4(f,a,b,ya,M).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

h=(b-a)/M;
T=zeros(1,M+1);
Y=zeros(1,M+1);
T=a:h:b;
Y(1)=ya;
for j=1:M
   k1=h*f(T(j),Y(j));
   k2=h*f(T(j)+h/2,Y(j)+k1/2);
   k3=h*f(T(j)+h/2,Y(j)+k2/2);
   k4=h*f(T(j)+h,Y(j)+k3);
   Y(j+1)=Y(j)+(k1+2*k2+2*k3+k4)/6;
end

R=[T' Y'];

