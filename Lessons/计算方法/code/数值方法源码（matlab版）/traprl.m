function s=traprl(f,a,b,M)

%Input      - f is the integrand 
%              - a and b are upper and lower limits of integration
%              - M is the number of subintervals
%Output   - s is the trapezoidal rule sum

%If f is defined as an M-file function use the @ notation
% call s=trprl(@f,a,b,M).
%If f is defined as an anonymous function use the
% call s=trprl(f,a,b,M).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

h=(b-a)/M;
s=0;

for k=1:(M-1)
   x=a+h*k;
   s=s+f(x);
end

s=h*(f(a)+f(b))/2+h*s;

