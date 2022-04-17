j=0;
inc=200;

for i=1:40
pr(i,:)=prec(j+1:inc+j);
op(i,:)=open(j+1:inc+j);
pt(i,:)=point(j+1:inc+j);
cl(i,:)=close(j+1:inc+j);
lt(i,:)=lateral(j+1:inc+j);
j=i*inc;
prfeat(i,:)=featurext(pr(i,:));
opfeat(i,:)=featurext(op(i,:));
clfeat(i,:)=featurext(cl(i,:));
ptfeat(i,:)=featurext(pt(i,:));
ltfeat(i,:)=featurext(lt(i,:));
end
data=[prfeat;opfeat;clfeat;ptfeat;ltfeat];
data=[data zeros(size(data,1),1)];
x=40;
j=0;
for i=1:5
   data(j+1:j+x,5)=i;
   j=i*x;
   end