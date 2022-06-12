function D=qr2(A,epsilon)

%Input    - A is a symmetric tridiagonal nxn matrix
%            - epsilon is the tolerance
%Output - D is the nx1 vector of eigenvalues

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Initialize parameters

[n,n]=size(A);
m=n;
D=zeros(n,1);
B=A;

while (m>1)
   while (abs(B(m,m-1))>=epsilon)
     
      %Calculate shift
      S=eig(B(m-1:m,m-1:m));
      [j,k]=min([abs(B(m,m)*[1 1]'-S)]);
      
      %QR factorization of B
      [Q,U]=qr(B-S(k)*eye(m));
      
      %Calculate next B
      B=U*Q+S(k)*eye(m);      
   end
   
   %Place mth eigenvalue in A(m,m)
   A(1:m,1:m)=B;
   
   %Repeat process on the m-1 x m-1 submatrix of A
   m=m-1;   
   B=A(1:m,1:m);   
end

D=diag(A);

