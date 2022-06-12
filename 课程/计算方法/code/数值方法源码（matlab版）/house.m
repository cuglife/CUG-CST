function T=house (A)

%Input - A is an nxn symmetric matrix
%Output - T is a tridiagonal matrix

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

[n,n]=size(A);

for k=1:n-2
   s=norm(A(k+1:n,k));
   if (A(k+1,k)<0)
      s=-s;
   end
   r=sqrt(2*s*(A(k+1,k)+s));
   W(1:k)=zeros(1,k);
   W(k+1)=(A(k+1,k)+s)/r;
   W(k+2:n)=A(k+2:n,k)'/r;
   V(1:k)=zeros(1,k);
   V(k+1:n)=A(k+1:n,k+1:n)*W(k+1:n)';
   c=W(k+1:n)*V(k+1:n)';
   Q(1:k)=zeros(1,k);
   Q(k+1:n)=V(k+1:n)-c*W(k+1:n);
   A(k+2:n,k)=zeros(n-k-1,1);
   A(k,k+2:n)=zeros(1,n-k-1);
   A(k+1,k)=-s;
   A(k,k+1)=-s;
   A(k+1:n,k+1:n)=A(k+1:n,k+1:n) ...
      -2*W(k+1:n)'*Q(k+1:n)-2*Q(k+1:n)'*W(k+1:n);
end

T=A;

