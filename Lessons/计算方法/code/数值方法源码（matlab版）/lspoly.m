function C = lspoly(X,Y,M)

%Input     - X is the 1xn abscissa vector
%             - Y is the 1xn ordinate vector
%             - M is the degree of the least-squares polynomial
% Output - C is the coefficient list for the polynomial

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

n=length(X);
B=zeros(1:M+1);
F=zeros(n,M+1);

%Fill the columns of F with the powers of X

for k=1:M+1
   F(:,k)=X'.^(k-1);
end

%Solve the linear system from (25)

A=F'*F;
B=F'*Y';
C=A\B;
C=flipud(C);

