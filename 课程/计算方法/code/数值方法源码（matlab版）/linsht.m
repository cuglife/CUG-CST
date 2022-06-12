function L=linsht(F1,F2,a,b,alpha,beta,M)

%Input    - F1 and F2 are the systems of first-order equations 
%              representing the I.V.P.'s (9) and (10), respectively;
%              input as M-file functions
%            - a and b are the endpoints of the interval
%            - alpha = x(a) and beta = x(b); the boundary conditions
%            - M is the number of steps
%Output - L = [T' X]; where T' is the (M+1) x 1 vector of abscissas 
%              and X is the (M+1) x 1 vector of ordinates

%Call L=linsht(@F1,@F2,a,b,alpha,beta,M)

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Solve the system F1

Za=[alpha,0];
[T,Z]=rks4(F1,a,b,Za,M);
U=Z(:,1);

%Solve the system F2

Za=[0,1];
[T,Z]=rks4(F2,a,b,Za,M);
V=Z(:,1);

%Calculate the solution to the boundary value problem

X=U+(beta-U(M+1))*V/V(M+1);
L=[T' X];

