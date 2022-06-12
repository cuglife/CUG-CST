function [C,X,Y]=cheby(fun,n,a,b)

%Input     - fun is the function to be approximated
%             - n is the degree of the Chebyshev interpolating polynomial
%             - a is the left endpoint
%             - b is the right endpoint
%Output  - C is the coefficient list for the polynomial
%             - X contains the abscissas
%             - Y contains the ordinates

% If fun is defined as an M-file function use the call
% [C,X,Y]=cheby('fun(x)',m,a,b)

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

if nargin==2, a=-1;b=1;end
d=pi/(2*n+2);
C=zeros(1,n+1);

for k=1:n+1
   X(k)=cos((2*k-1)*d);
end

X=(b-a)*X/2+(a+b)/2;
x=X;
Y=eval(fun);

for k =1:n+1
   z=(2*k-1)*d;
   for j=1:n+1
      C(j)=C(j)+Y(k)*cos((j-1)*z);
   end
end

C=2*C/(n+1);
C(1)=C(1)/2;

   