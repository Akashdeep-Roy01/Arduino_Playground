function CosA = CalcAngle(x,y)
theta1=80;
phi1=60;
theta3=140 ;
phi3=190;
fixed=25;


        syms k1 k2 k3;
        eqn1=k2*cosd(phi1)-k1*cosd(theta1) + k3 == cosd(-phi1+theta1);
        eqn2=k2*cosd(x)-k1*cosd(y) + k3 == cosd(-x+y);
        eqn3=k2*cosd(phi3)-k1*cosd(theta3) + k3 == cosd(-phi3+theta3);
        sol = solve([eqn1, eqn2, eqn3], [k1, k2, k3]);
        k1Sol = sol.k1;
        k2Sol = sol.k2;
        k3Sol = sol.k3;
        k1=double(k1Sol);
        k2=double(k2Sol);
        k3=double(k3Sol);
        input=fixed/k2;
        output=fixed/k1;
        coupler= sqrt(input^2 + output^2 + fixed^2 - 2*input*output*k3);
        F=(-fixed^2-input^2+coupler^2+output^2-2*fixed*input*cosd(y))/(2*output*coupler);
        CosA=F;
end
