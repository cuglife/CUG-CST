function A=abm(f,T,Y)

%Input   - f is the function 
%           - T is the vector of abscissas
%           - Y is the vector of ordinates
%Remark.  The first four coordinates of T and Y must
%               have starting values obtained with RK4
%Output - A=[T' Y'] where T is the vector of abscissas and
%              Y is the vector of ordinates

% If f is an M-file function call A=abm(@,f,T,Y).
% If f is an anonymous function call A=abm(f,T,Y).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

n=length(T);
if n<5,return, end;

F=zeros(1,4);
F=f(T(1:4),Y(1:4));
h=T(2)-T(1);

for k=4:n-1
   %Predictor
   p=Y(k)+(h/24)*(F*[-9 37 -59 55]');
   T(k+1)=T(1)+h*k;
   F=[F(2) F(3) F(4) f(T(k+1),p)];
   %Corrector
   Y(k+1)=Y(k)+(h/24)*(F*[1 -5 19 9]');
   F(4)=f(T(k+1),Y(k+1));
end

A=[T' Y'];

