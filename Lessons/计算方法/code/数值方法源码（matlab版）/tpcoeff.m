function [A,B]=tpcoeff(X,Y,M)

%Input     - X is a vector of equally spaced abscisssas in [-pi, pi]
%             - Y is a vector of ordinates
%             - M is the degree of the trigomometric polynomial
%Output  - A is a vector containing the coefficients of cos(jx)
%             - B is a vector containing the coefficients of sin(jx)

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

N=length(X)-1;
max1=fix((N-1)/2);

if M>max1
   M=max1;
end

A=zeros(1,M+1);
B=zeros(1,M+1);
Yends=(Y(1)+Y(N+1))/2;
Y(1)=Yends;
Y(N+1)=Yends;
A(1)=sum(Y);

for j=1:M
   A(j+1)=cos(j*X)*Y';
   B(j+1)=sin(j*X)*Y';
end

A=2*A/N;
B=2*B/N;
A(1)=A(1)/2;

