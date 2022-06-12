function U=dirich(f1,f2,f3,f4,a,b,h,tol,max1)

%Input    - f1,f2,f3,f4 are boundary functions 
%            - a and b right endpoints of [0,a] and [0,b]
%            - h step size
%            - tol is the tolerance
%            - max1 is the number of iteration
%Output - U solution matrix; analogous to Table 10.6

% If f1, f2, f3, and f4 are M-file functions call
% U=dirich(@f1,@f2,@f3,@f4,a,b,h,tol,max1).
% If f1, f2, f3, and f4 are anonymous functions call
% U=dirich(f1,f2,f3,f4,a,b,h,tol,max1)

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Initialize parameters and U

n=fix(a/h)+1;
m=fix(b/h)+1;
ave=(a*(f1(0)+f2(0)) ...
   +b*(f3(0)+f4(0)))/(2*a+2*b);
U=ave*ones(n,m);

%Boundary conditions

U(1,1:m)=f3(0:h:(m-1)*h)';
U(n,1:m)=f4(0:h:(m-1)*h)';
U(1:n,1)=f1(0:h:(n-1)*h);
U(1:n,m)=f2(0:h:(n-1)*h);
U(1,1)=(U(1,2)+U(2,1))/2;
U(1,m)=(U(1,m-1)+U(2,m))/2;
U(n,1)=(U(n-1,1)+U(n,2))/2;
U(n,m)=(U(n-1,m)+U(n,m-1))/2;

%SOR parameter

w=4/(2+sqrt(4-(cos(pi/(n-1))+cos(pi/(m-1)))^2));

%Refine approximations and sweep operator throughout the grid

err=1;
cnt=0;
while((err>tol)&(cnt<=max1))
   err=0;
   for j=2:m-1
      for i=2:n-1
         relx=w*(U(i,j+1)+U(i,j-1)+U(i+1,j)+U(i-1,j)-4*U(i,j))/4;
         U(i,j)=U(i,j)+relx;
         if (err<=abs(relx))
           err=abs(relx);
         end
      end   
   end
cnt=cnt+1;
end

U=flipud(U');

