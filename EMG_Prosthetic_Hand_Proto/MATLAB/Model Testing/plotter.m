%function [data , T]= plotter(a)
%tic
Vol=zeros(1000,1);
for i= 1:1000
    Vol(i)= readVoltage(a,'A0');%*5/1023;
    plot(Vol)
    title('Emg Plot')
    xlabel('Sample')
    ylabel('Volts')
    grid on;
    drawnow;
    data=Vol;
end
%T=toc;

    