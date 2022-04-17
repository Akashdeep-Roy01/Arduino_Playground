function accu = wavel(d)
accu=0;
n=size(d,2);
for i=1:n-1
    delta=abs(d(i+1)-d(i));
    accu=accu+delta;
end
