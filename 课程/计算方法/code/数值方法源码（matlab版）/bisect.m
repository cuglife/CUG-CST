function [c,err,yc]=bisect(f,a,b,delta)

%Input    - f is the function 
%	         - a and b are the left and right endpoints
%	         - delta is the tolerance
%Output - c is the zero
%	         - yc= f(c)
% 	         - err is the error estimate for c

%If f is defined as an M-file function use the @ notation
% call [c,err,yc]=bisect(@f,a,b,delta).
%If f is defined as an anonymous function use the
% call [c,err,yc]=bisect(f,a,b,delta).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

ya=f(a);
yb=f(b);
if ya*yb > 0,return,end
max1=1+round((log(b-a)-log(delta))/log(2));
for k=1:max1
	c=(a+b)/2;
	yc=f(c);
	if yc==0
		a=c;
		b=c;
	elseif yb*yc>0
		b=c;
		yb=yc;
	else
		a=c;
		ya=yc;
	end
	if b-a < delta, break,end
end

c=(a+b)/2;
err=abs(b-a);
yc=f(c);

