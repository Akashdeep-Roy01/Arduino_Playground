function [feat] = featurext(data)
mn=mean(data);
vr=var(data);
wl=wavel(data);
s=ssc(data);
feat=[mn vr wl s];