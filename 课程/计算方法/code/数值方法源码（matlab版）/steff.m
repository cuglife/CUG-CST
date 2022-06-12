function [p,Q]=steff(f,df,p0,delta,epsilon,max1)

%Input    - f is the object function 
%	         - df is the derivative of f input as a string 'df'
%            - p0 is the initial approximation to a zero of f
%            - delta is the tolerance for p0
%	         - epsilon is the tolerance for the function values y
%	         - max1 is the maximum number of iterations
%Output - p is the Steffensen approximation to the zero
%   	     - Q is the matrix containing the Steffensen sequence

%If f and df are defined as M-file functions use the @ notation
% call [p,Q]=steff(@f,@df,p0,delta,epsilon,max1).
%If f and df are defined as anonymous functions use the
% call  [p,Q]=steff(f,df,p0,delta,epsilon,max1).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Initialize the matrix R

R=zeros(max1,3);
R(1,1)=p0;

for k=1:max1
  for j=2:3

    %Denominator in Newton-Raphson method is calculated
    nrdenom=df(R(k,j-1));

    %Conditional calculates Newton-Raphson approximations
    if nrdenom==0
      'division by zero in Newton-Raphson method'
       break		
    else
      R(k,j)=R(k,j-1)-f(R(k,j-1))/nrdenom;
    end

    %Denominator in Aitkens Acceleration process is calculated
    aadenom=R(k,3)-2*R(k,2)+R(k,1);

    %Conditional calculates Aitkens Acceleration approximations
    if aadenom==0
      'division by zero in Aitkens Acceleration'
      break		
    else
      R(k+1,1)=R(k,1)-(R(k,2)-R(k,1))^2/aadenom;
    end
   end

   %Breaks out and ends program if division by zero occured
   if (nrdenom==0)|(aadenom==0)
     break
   end

   %Stopping criteria are evaluated; p and the matrix Q are determined	
    err=abs(R(k,1)-R(k+1,1));
    relerr=err/(abs(R(k+1,1))+delta);
    y=f(R(k+1,1));
    if (err<delta)|(relerr<delta)|(y<epsilon)
       p=R(k+1,1);
       Q=R(1:k+1,:);
       break
    end
end

