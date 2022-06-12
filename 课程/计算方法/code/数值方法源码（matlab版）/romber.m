function [R,quad,err,h]=romber(f,a,b,n,tol)

%Input    - f is the integrand 
%            - a and b are upper and lower limits of integration
%            - n is the maximum number of rows in the table
%            - tol is the tolerance
%Output - R is the Romberg table
%           - quad is the quadrature value
%           - err is the error estimate
%           - h is the smallest step size used


%If f is defined as an M-file function use the @ notation
% call [R,quad,err,h]=romber(@f,a,b,n,tol).
%If f is defined as an anonymous function use the
% call [R,quad,err,h]=romber(f,a,b,n,tol).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

M=1;
h=b-a;
err=1;
J=0;
R=zeros(4,4);
R(1,1)=h*(f(a)+f(b))/2;

while((err>tol)&(J<n))|(J<4)
   J=J+1;
   h=h/2;
   s=0;
   for p=1:M
      x=a+h*(2*p-1);
      s=s+f(x);
   end
   R(J+1,1)=R(J,1)/2+h*s;
   M=2*M;
   for K=1:J
      R(J+1,K+1)=R(J+1,K)+(R(J+1,K)-R(J,K))/(4^K-1);
   end
   err=abs(R(J,J)-R(J+1,K+1));
end

quad=R(J+1,J+1);

