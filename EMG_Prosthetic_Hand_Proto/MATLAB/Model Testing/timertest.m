
t = timer('TimerFcn', 'stat=false; disp(''Timer!'')',... 
                 'StartDelay',1);
start(t)
i=1;
stat=true;
Vol=zeros(1000,1);
while(stat==true)
  Vol(i)=readVoltage(a,'A0');%*5/1023;
  i=i+1;
end
delete(t)
i
