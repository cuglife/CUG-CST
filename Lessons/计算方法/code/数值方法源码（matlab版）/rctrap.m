function T=rctrap(f,a,b,n)

%Input    - f is the integrand 
%            - a and b are upper and lower limits of integration
%            - n is the number of times for recursion
%Output - T is the recursive trapezoidal rule list

%If f is defined as an M-file function use the @ notation
% call T=rctrap(@f,a,b,n).
%If f is defined as an anonymous function use the
% call T=rctrap(f,a,b,n).

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
T=zeros(1,n+1);
T(1)=h*(f(a)+f(b))/2;

for j=1:n
   M=2*M;
   h=h/2;
   s=0;
   for k=1:M/2
      x=a+h*(2*k-1);
      s=s+f(x);
   end
   T(j+1)=T(j)/2+h*s;
end

