function [D,err,relerr,n]=diffext(f,x,delta,toler)

%Input    - f is the function 
%            - x is the differentiation point
%            - delta is the tolerance for the error
%            - toler is the tolerance for the relative error
%Output - D is the matrix of approximate derivatives
%            - err is the error bound
%            - relerr is the relative error bound
%            - n is the coordiante of the "best approximation"

% If f is an M-file function use the call [D,err,relerr,n]=diffext(@f,x,delta,toler).
% If f is an anonymous function use the call [D,err,relerr,n]=diffext(f,x,delta,toler).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

err=1;
relerr=1;
h=1;
j=1;
D(1,1)=(f(x+h)-f(x-h))/(2*h);

while relerr > toler & err > delta &j <12
   h=h/2;
   D(j+1,1)=(f(x+h)-f(x-h))/(2*h);
   for k=1:j
      D(j+1,k+1)=D(j+1,k)+(D(j+1,k)-D(j,k))/((4^k)-1);
   end
   err=abs(D(j+1,j+1)-D(j,j));
   relerr=2*err/(abs(D(j+1,j+1))+abs(D(j,j))+eps);
   j=j+1;
end

[n,n]=size(D);

