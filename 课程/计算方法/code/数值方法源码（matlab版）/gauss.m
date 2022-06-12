function quad=gauss(f,a,b,A,W)

%Input     - f is the integrand 
%             - a and b upper and lower limits of integration
%             - A is the 1 x N vector of abscissas from Table 7.9
%             - W is the 1 x N vector of weights from Table 7.9
%Output  - quad is the quadrature value

%If f is defined as an M-file function use the @ notation
% call quad=gauss(@f,a,b,A,W).
%If f is defined as an anonymous function use the
% call quad=gauss(f,a,b,A,W).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

N=length(A);
T=zeros(1,N);
T=((a+b)/2)+((b-a)/2)*A;
quad=((b-a)/2)*sum(W.*f(T));

