theta1=80;
phi1=60;
theta3=140 ;
phi3=190;
fixed=25;
i=1;

for phi2=phi1:phi3-1
    for theta2=theta1:theta3-1
        T=CalcAngle(phi2,theta2);
        if (T > 0) && (T < 1)     
             TAngle(i)=T;
             phi(i)=phi2;
             theta(i)=theta2;
             i=i+1;
        end
    end
end
[M I] = min(TAngle);        
theta2=theta(I);
phi2=phi(I);
syms k1 k2 k3;
        eqn1=k2*cosd(phi1)-k1*cosd(theta1) + k3 == cosd(-phi1+theta1);
        eqn2=k2*cosd(phi2)-k1*cosd(theta2) + k3 == cosd(-phi2+theta2);
        eqn3=k2*cosd(phi3)-k1*cosd(theta3) + k3 == cosd(-phi3+theta3);
        sol = solve([eqn1, eqn2, eqn3], [k1, k2, k3]);
        k1Sol = sol.k1;
        k2Sol = sol.k2;
        k3Sol = sol.k3;
        k1=double(k1Sol);
        k2=double(k2Sol);
        k3=double(k3Sol);
        output=fixed/k1
        input=fixed/k2
        coupler= sqrt(input^2 + output^2 + fixed^2 - 2*input*output*k3)
        
        
