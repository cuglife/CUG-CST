function X=trisys (A,D,C,B)

%Input    - A is the sub diagonal of the coefficient matrix
%            - D is the main diagonal of the coefficient matrix
%            - C is the super diagonal of the coefficient matrix
%            - B is the constant vector of the linear system
%Output - X is the solution vector

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

N=length(B);
for k=2:N
   mult=A(k-1)/D(k-1);
   D(k)=D(k)-mult*C(k-1);
   B(k)=B(k)-mult*B(k-1);
end

X(N)=B(N)/D(N);

for k= N-1:-1:1
   X(k)=(B(k)-C(k)*X(k+1))/D(k);
end

