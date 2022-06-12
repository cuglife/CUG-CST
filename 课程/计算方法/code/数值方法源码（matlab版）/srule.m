function Z=srule(f,a0,b0,tol0)

%Input     - f is the integrand 
%             - a0 and b0 are upper and lower limits of integration
%             - tol0 is the tolerance
% Output - Z is a 1 x 6 vector [a0 b0 S S2 err tol1] 

%If f is defined as an M-file function use the @ notation
% call Z=srule(@f,a0,b0,tol0).
%If f is defined as an anonymous function use the
% call Z=srule(f,a0,b0,tol0).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

h=(b0-a0)/2;
C=zeros(1,3);
C=f([a0 (a0+b0)/2 b0]);
S=h*(C(1)+4*C(2)+C(3))/3;
S2=S;
tol1=tol0;
err=tol0;

Z=[a0 b0 S S2 err tol1];

