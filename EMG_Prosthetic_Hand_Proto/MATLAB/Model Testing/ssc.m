function count= ssc(d)
count=0;
n=size(d,2)-2;
for i=1:n
    if d(i+1)>d(i) && d(i+1)>d(i+2)
        count=count+1;
    elseif d(i+1)<d(i) && d(i+1)<d(i+2)
        count=count+1;
    end
end