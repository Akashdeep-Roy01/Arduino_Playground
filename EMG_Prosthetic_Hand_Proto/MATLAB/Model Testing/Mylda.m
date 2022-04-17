[n m] = size(Input);
ClassLabel = unique(Target);
k = length(ClassLabel);
for i = 1:k
    Group      = (Target == ClassLabel(i));
    nGroup(i)  = sum(double(Group));
    GroupMean(i,:) = mean(Input(Group,:));
end
for i=1:k
    Meandiff(i,:)= 2*GroupMean(i,:)-sum(GroupMean);
end
Coef=Meandiff/In;