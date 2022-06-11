function R = approot (f,X,epsilon)

% Input    - f is object function 
%             - X is the vector of abscissas
%             - epsilon is the tolerance 
% Output - R is the vector of approximate locations for roots

% If f is an M-file function call R = approot (@f,X,epsilon).
% If f is an anonymous function call R = approot (f,X,epsilon).

%  NUMERICAL METHODS: Matlab Programs
% (c) 2004 by John H. Mathews and Kurtis D. Fink
%  Complementary Software to accompany the textbook:
%  NUMERICAL METHODS: Using Matlab, Fourth Edition
%  ISBN: 0-13-065248-2
%  Prentice-Hall Pub. Inc.
%  One Lake Street
%  Upper Saddle River, NJ 07458

Y=f(X);
yrange = max(Y)-min(Y);
epsilon2 = yrange*epsilon;
n=length(X);
m=0;
X(n+1)=X(n);
Y(n+1)=Y(n);

for k=2:n,
	if Y(k-1)*Y(k) <= 0,
		m=m+1;
		R(m)=(X(k-1)+X(k))/2;
	end
	s=(Y(k)-Y(k-1))*(Y(k+1)-Y(k));
	if (abs(Y(k)) < epsilon2) & (s <= 0),
		m=m+1;
		R(m)=X(k);
	end
end

