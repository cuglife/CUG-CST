function [V,D]=jacobi1(A,epsilon)

%Input    - A is an nxn matrix
%            - epsilon the is tolerance
%Output - V is the nxn matrix of eigenvectors
%            - D is the diagonal nxn matrix of eigenvalues

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Initialize V, D, and parameters

D=A;
[n,n]=size(A);
V=eye(n);
state=1;

%Calculate row p and column q of the off-diagonal element
%of greatest magnitude in A

[m1 p]=max(abs(D-diag(diag(D))));
[m2 q]=max(m1);
p=p(q);

while (state==1)
   %Zero out Dpq and Dqp
   t=D(p,q)/(D(q,q)-D(p,p));
   c=1/sqrt(t^2+1);
   s=c*t;
   R=[c s;-s c];
   D([p q],:)=R'*D([p q],:);
   D(:,[p q])=D(:,[p q])*R;
   V(:,[p q])=V(:,[p q])*R;
   [m1 p]=max(abs(D-diag(diag(D))));
   [m2 q]=max(m1);
   p=p(q);
   if (abs(D(p,q))<epsilon*sqrt(sum(diag(D).^2)/n))
      state=0;
   end
end

D=diag(diag(D));

