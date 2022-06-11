function E=euler(f,g,a,b,xa,ya,M)
%Input - f is the function entered as a string 'f'
%      - g is the function entered as a string 'g'
%         - a and b are the left and right endpoints
%         - xa is the initial condition x(a)
%         - ya is the initial condition y(a)
%         - M is the number of steps
%Output - E=[T' X' Y'] where T is the vector of abscissas 
%              and X and Y are the vectors of ordinates
h=(b-a)/M;
T=zeros(1,M+1);
X=zeros(1,M+1);
Y=zeros(1,M+1);
T=a:h:b;
X(1)=xa;
Y(1)=ya;
for j=1:M
X(j+1)=X(j)+h*feval(f,T(j),X(j),Y(j));
Y(j+1)=Y(j)+h*feval(g,T(j),X(j),Y(j));
end
E=[T' X' Y'];