function [p,yp,dp,dy,P] = quadmin(f,a,b,delta,epsilon)

%Input     - f is the object function
%             - a and b are the endpoints of the interval 
%             - delta is the tolerance for the abscissas
%             - epsilon is the tolerance for the ordinates
%Output  - p is the abscissa of the minimum
%             - yp is the ordinate of the minimum
%             - dp is the error bound for  p
%             - dy is the error bound for yp
%             - P is the vector of iterations

%If f is defined as an M-file function use the @ notation
% call [p,yp,dp,dy,P] = quadmin(@f,a,b,delta,epsilon).
%If f is defined as an anonymous function use the
% call [p,yp,dp,dy,P] = quadmin(f,a,b,delta,epsilon).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

p0 = a;
maxj = 20;
maxk = 30;
big = 1e6;
err = 1;
k = 1;
P(k) = p0;
cond = 0;
h = 1;

if (abs(p0)>1e4), h = abs(p0)/1e4; end
while (k<maxk & err>epsilon & cond~=5)
  f1 = (f(p0+0.00001)-f(p0-0.00001))/0.00002;
  if (f1>0), h = -abs(h); end
  p1 = p0 + h;
  p2 = p0 + 2*h;
  pmin = p0;
  y0 = f(p0);
  y1 = f(p1);
  y2 =f(p2);
  ymin = y0;
  cond = 0;
  j = 0;
  
  %Determine h so that y1<y0 & y1<y2
  
  while (j<maxj & abs(h)>delta & cond==0)
    if (y0<=y1),
      p2 = p1;
      y2 = y1;
      h = h/2;
      p1 = p0 + h;
      y1 = f(p1);
    else
      if (y2<y1),
        p1 = p2;
        y1 = y2;
        h = 2*h;
        p2 = p0 + 2*h;
        y2 = f(p2);
      else
        cond = -1;
      end
    end
    j = j+1;
    if (abs(h)>big | abs(p0)>big), cond=5; end
  end
  if (cond==5),
    pmin = p1;
    ymin =f(p1);
 else
    
    %Quadratic interpolation to find yp
    d = 4*y1-2*y0-2*y2;     
    if (d<0),
      hmin = h*(4*y1-3*y0-y2)/d;
    else
      hmin = h/3;
      cond = 4;
    end
    pmin = p0 + hmin;
    ymin =f(pmin);
    h = abs(h);
    h0 = abs(hmin);
    h1 = abs(hmin-h);
    h2 = abs(hmin-2*h);
    
    %Determine magnitude of next h
    if (h0<h),  h = h0;   end
    if (h1<h),  h = h1;   end
    if (h2<h),  h = h2;   end
    if (h==0),  h = hmin; end
    if (h<delta), cond=1; end
    if (abs(h)>big | abs(pmin)>big), cond=5; end
    
    %Termination test for minimization
    e0 = abs(y0-ymin);
    e1 = abs(y1-ymin);
    e2 = abs(y2-ymin);
    if (e0~=0 & e0<err), err = e0; end
    if (e1~=0 & e1<err), err = e1; end
    if (e2~=0 & e2<err), err = e2; end
    if (e0~=0 & e1==0 & e2==0), error=0; end
    if (err<epsilon), cond=2; end
    p0 = pmin;
    k = k+1;
    P(k) = p0;
  end                           
  if (cond==2 & h<delta), cond=3; end
end

p = p0;
dp = h;
yp = f(p);
dy = err;

