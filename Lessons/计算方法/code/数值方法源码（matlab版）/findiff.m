function F=findiff(p,q,r,a,b,alpha,beta,N)

%Input    - p, q, and r are the coefficient functions of (1)
%            - a and b are the left and right endpoints
%            - alpha =x(a) and beta=x(b)
%            - N is the number of steps
%Output - F=[T' X']:wherer T' is the 1xN vector of abscissas and
%              X' is the 1xN vector of ordinates.

% If p, q, and r are M-file functions 
%call F=findiff(@p,@q,@r,a,b,alpha,beta,N).
% If p, q, and r are anonymous functions 
%call F=findiff(p,q,r,a,b,alpha,beta,N)

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

%Initialize vectors and h

T=zeros(1,N+1);
X=zeros(1,N-1);
Va=zeros(1,N-2);
Vb=zeros(1,N-1);
Vc=zeros(1,N-2);
Vd=zeros(1,N-1);
h=(b-a)/N;

%Calculate the constant vector B in AX=B

Vt=a+h:h:a+h*(N-1);
Vb=-h^2*r(Vt);
Vb(1)=Vb(1)+(1+h/2*p(Vt(1)))*alpha;
Vb(N-1)=Vb(N-1)+(1-h/2*p(Vt(N-1)))*beta;

%Calculate the main diagonal of A in AX=B

Vd=2+h^2*q(Vt);

%Calculate the super diagonal of A in AX=B

Vta=Vt(1,2:N-1);
Va=-1-h/2*p(Vta);

%Calculate the sub diagonal of A in AX=B

Vtc=Vt(1,1:N-2);
Vc=-1+h/2*p(Vtc);

%Solve AX=B using trisys

X=trisys(Va,Vd,Vc,Vb);
T=[a,Vt,b];
X=[alpha,X,beta];
F=[T' X'];

