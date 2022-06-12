function langrange= langrange( x,n )
langrange=0;
xx=linspace(-4,4,n+1);
for i=1:n+1
    lix=1;
    for j=1:n+1
        if j~=i
            lix=lix.*((x-xx(j))./(xx(i)-xx(j)));
        end
    end
    langrange=f(xx(i)).*lix+langrange;
end
end