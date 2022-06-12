function U=crnich(f,c1,c2,a,b,c,n,m)

%Input   - f=u(x,0) 
%            - c1=u(0,t) and c2=u(a,t)
%            - a and b right endpoints of [0,a] and [0,b]
%            - c the constant in the heat equation
%            - n and m number of grid points over [0,a] and [0,b]
%Output - U solution matrix; analogous to Table 10.5

%If f is an M-file function call U=crnich(@f,c1,c2,a,b,c,n,m).
%If f is an anonymous function call U=crnich(f,c1,c2,a,b,c,n,m).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Initialize parameters and U

h=a/(n-1);
k=b/(m-1);
r=c^2*k/h^2;
s1=2+2/r;
s2=2/r-2;
U=zeros(n,m);

%Boundary conditions

U(1,1:m)=c1;
U(n,1:m)=c2;

%Generate first row

U(2:n-1,1)=f(h:h:(n-2)*h)';

%Form the diagonal and off-diagonal elemnts of A and 
%the constant vector B and solve tridiagonal system AX=B

Vd(1,1:n)=s1*ones(1,n);
Vd(1)=1;
Vd(n)=1;
Va=-ones(1,n-1);
Va(n-1)=0;
Vc=-ones(1,n-1);
Vc(1)=0;
Vb(1)=c1;
Vb(n)=c2;
for j=2:m
   for i=2:n-1
      Vb(i)=U(i-1,j-1)+U(i+1,j-1)+s2*U(i,j-1);
   end
   X=trisys(Va,Vd,Vc,Vb);
   U(1:n,j)=X';
end

U=U';

