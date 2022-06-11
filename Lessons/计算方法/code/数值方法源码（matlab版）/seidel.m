function [P,iter]= seidel(G,P,delta, max1)

%Input    -  G is the nonlinear fixed-point system 
%               saved as an M-file function
%            -  P is the initial guess at the solution 
%            -  delta is the error bound
%            - max1 is the number of iterations
%Output - P is the seidel approximation to the solution
%     	     - iter is the number of iterations required

%Use the @ notation call [P,iter]=seidel(@G, P, delta, ma).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

N=length(P);

for k=1:max1
   X=P;
   
   % X is the kth approximation to the solution
   for j=1:N     
      A=G(X);
      % Update the terms of X as they are calculated
      X(j)=A(j);     
   end

   err=abs(norm(X-P));
   relerr=err/(norm(X)+eps);
   P=X;
   iter=k;
   if (err<delta)|(relerr<delta)
     break
   end
end

