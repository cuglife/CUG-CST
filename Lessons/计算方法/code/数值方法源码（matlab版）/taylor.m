function T4=taylor(df,a,b,ya,M)

%Input    - df=[y' y'' y''' y''''] where y'=f(t,y)
%            - a and b are the left and right endpoints
%            - ya is the initial condition y(a)
%            - M is the number of steps
%Output - T4=[T' Y'] where T is the vector of abscissas and
%            - Y is the vector of ordinates

%If df is an M-file function call T4=taylor(@df,a,b,ya,M).
%If df is an anonymous function call T4=taylor(df,a,b,ya,M).

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
   D=df(T(j),Y(j));
   Y(j+1)=Y(j)+h*(D(1)+h*(D(2)/2+h*(D(3)/6+h*D(4)/24)));
end

T4=[T' Y'];

