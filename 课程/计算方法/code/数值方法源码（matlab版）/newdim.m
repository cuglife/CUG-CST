function [P,iter,err]=newdim(F,JF,P,delta,epsilon,max1)

%Input    -F is the system saved as the M-file F.m
%            -JF is the Jacobian of F saved as the M-file JF.M
%            -P is the inital approximation to the solution
%            -delta is the tolerance for P
%            -epsilon is the tolerance for F(P)
%            -max1 is the maximum number of iterations
%Output -P is the approximation to the solution
%            -iter is the number of iterations required
%            -err is the error estimate for P

%Use the @ notation call
%[P,iter,err]=newdim(@F, @JF, P, delta, epsilon, max1).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

Y=F(P);

for k=1:max1
   J=JF(P);
   Q=P-(J\Y')';
   Z=F(Q);
   err=norm(Q-P);
   relerr=err/(norm(Q)+eps);
   P=Q;
   Y=Z;
   iter=k;
   if (err<delta)|(relerr<delta)|(abs(Y)<epsilon)
     break
   end
end



