clear;
a=arduino;
load('Final.mat')

while 1
    for i= 1:200
    Vol(i)= readVoltage(a,'A0');%*5/1023;
    end
    feat=featurext(Vol);
    X=feat;
    Output=W*[1 X]';
    [M I]=max(Output);
    p=I;
    %p=trainedModel.predictFcn(X);
    if p==3
        disp('Close')
    elseif p==2
        disp('Open')
    elseif p==1
        disp('Precision')
    elseif p==4
        disp('Point')
    end
    i=0;
end