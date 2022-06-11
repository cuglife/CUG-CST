function [SRmat,quad,err]=adapt(f,a,b,tol)

%Input    - f is the integrand
%            - a and b are upper and lower limits of integration
%            - tol is the tolerance
%Output - SRmat is the table of values
%           - quad is the quadrature value
%           - err is the error estimate

%If f is defined as an M-file function use the @ notation
% call [SRmat,quad,err]=adapt(@f,a,b,tol).
%If f is defined as an anonymous function use the
% call [SRmat,quad,err]=adapt(f,a,b,tol).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Initialize values

SRmat = zeros(30,6);
iterating=0;
done=1;
SRvec=zeros(1,6);
SRvec=srule(f,a,b,tol);
SRmat(1,1:6)=SRvec;
m=1;
state=iterating;

while(state==iterating)
   n=m;
   for j=n:-1:1
      p=j;
      SR0vec=SRmat(p,:);
      err=SR0vec(5);
      tol=SR0vec(6);
      if (tol<=err)
         
         %Bisect interval,apply Simpson's rule
         %recursively, and determine error
         state=done;
         SR1vec=SR0vec;
         SR2vec=SR0vec;
         a=SR0vec(1);
         b=SR0vec(2);
         c=(a+b)/2;
         err=SR0vec(5);
         tol=SR0vec(6);
         tol2=tol/2;
         SR1vec=srule(f,a,c,tol2);
         SR2vec=srule(f,c,b,tol2);
         err=abs(SR0vec(3)-SR1vec(3)-SR2vec(3))/10;
         
         %Accuracy test
         if (err<tol)
            SRmat(p,:)=SR0vec;
            SRmat(p,4)=SR1vec(3)+SR2vec(3);
            SRmat(p,5)=err;
         else
            SRmat(p+1:m+1,:)=SRmat(p:m,:);
            m=m+1;
            SRmat(p,:)=SR1vec;
            SRmat(p+1,:)=SR2vec;
            state=iterating;
         end
      end
   end
end

quad=sum(SRmat(:,4));
err=sum(abs(SRmat(:,5)));
SRmat=SRmat(1:m,1:6);

