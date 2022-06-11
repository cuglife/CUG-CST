function X=backsub(A,B)

%Input    - A is an n x n upper-triangular nonsingular matrix
%	         - B is an n x 1 matrix
%Output - X is the solution to the linear system AX = B

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Find the dimension of B and initialize X
 n=length(B);
 X=zeros(n,1);
 X(n)=B(n)/A(n,n);

for k=n-1:-1:1
 X(k)=(B(k)-A(k,k+1:n)*X(k+1:n))/A(k,k);
end

  