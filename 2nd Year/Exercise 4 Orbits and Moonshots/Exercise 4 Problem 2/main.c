#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double axc(double x, double y);
double ayc(double x, double y);
double xinc(double x, double h, double k1x, double k2x, double k3x, double k4x);
double yinc(double y, double h, double k1y, double k2y, double k3y, double k4y);
double vxinc(double vx, double h, double k1vx, double k2vx, double k3vx, double k4vx);
double vyinc(double vy, double h, double k1vy, double k2vy, double k3vy, double k4vy);
double tinc(double t, double h);
//To get good shot, impulse is 3011.97 for 24km off the moon but crash into earth

int main()
{
    double x,y,vx,vy,v,M,rt,a,t,h,k1x,k1y,k1vx,k1vy,k2x,k2y,k2vx,k2vy,k3x,k3y,k3vx,k3vy,k4x,k4y,k4vx,k4vy, t_end, r0, re, GMe, x_imp, rm, r_m, r1;

    rt=7000000;//setting of initial conditions
    a=(M_PI);
    GMe = 3.9853545e+14;
    x = -rt;
    x_imp = 3011.97;
    vy = pow((GMe/rt),0.5)*-cos(a) + x_imp;//calculation for overall initial velocity component
    vx = pow((GMe/rt),0.5)*sin(a);
    y = 0;
    re=6371000;
    rm=1737400;//radius of moon
    r_m = 384400000;//distance from earth to the moon
    h=1;
    FILE *moonshot;

    moonshot=fopen("moonshot.txt","w");//file opened to write data to
     if (moonshot != NULL)
    {
        printf("File created containing the data from the moonshot\n");
        fprintf(moonshot, "Initial tangential velocity: %lf ms^-1 \t Radius of Orbit: 7000000 (m) \n\n\t Time (s) \t x (m) \t y (m) \t vx (ms^-1) \t vy (ms^-1) \n", v);
    }
    else
    {
        printf("Un-able to create file\n");
    }

    t=0;
    do
    {
    k1x = vx;//employing the Runge-Kutta method
    k1y = vy;
    k1vx = axc(x,y);
    k1vy = ayc(x,y);

    k2x = vx+(h*k1vx)/2;
    k2y = vy+(h*k1vy)/2;
    k2vx = axc(x+(h*k1x)/2,y+(h*k1y)/2);
    k2vy = ayc(x+(h*k1x)/2,y+(h*k1y)/2);

    k3x = vx+(h*k2vx)/2;
    k3y = vy+(h*k2vy)/2;
    k3vx = axc(x+(h*k2x)/2,y+(h*k2y)/2);
    k3vy = ayc(x+(h*k2x)/2,y+(h*k2y)/2);

    k4x = vx+(h*k3vx);
    k4y = vy+(h*k3vy);
    k4vx = axc(x+(h*k3x),y+(h*k3y));
    k4vy = ayc(x+(h*k3x),y+(h*k3y));

    x = xinc(x,h,k1x,k2x,k3x,k4x);
    y = yinc(y,h,k1y,k2y,k3y,k4y);
    vx = vxinc(vx,h,k1vx,k2vx,k3vx,k4vx);
    vy = vyinc(vy,h,k1vy,k2vy,k3vy,k4vy);
    t++;

    r0=pow(x*x+y*y,0.5);//distance from probe to earth
    r1=pow(pow((x-r_m),2)+y*y,0.5);//distance from probe to moon
    if (r0<=re)
    {
        printf("The satellite has crashed into earth!");
        break;
    }
    if (r1<=rm)
    {
        printf("The satellite has crashed into the moon!");
        break;
    }
    printf("%lf %lf %lf %lf\n", x, y, r1, t);
    fprintf(moonshot,"%lf\t%lf\t%lf\t%lf\n", t, x, y, r1);
    }while (t<=1000000);
    printf("\n\nProcess complete! Data written to moonshot.txt");

    return 0;
}


double axc(double x, double y)
{
   double ax, GMe, GMm, Rm;
   GMe = 3.9853545e+14;
   GMm = 4.90371e+12;
   Rm = 384400000;
   ax = -(GMe*x)/(pow((pow(x,2)+(pow(y,2))),1.5)) - (GMm*(x-Rm))/(pow((pow((x-Rm),2)+(pow(y,2))),1.5));//adjusted equation of motion to account for the moon
   return ax;
}

double ayc(double x, double y)
{

   double ay, GMe, GMm, Rm;
   GMe = 3.9853545e+14;
   GMm = 4.90371e+12;
   Rm = 384400000;
   ay = -(GMe*y)/(pow((pow(x,2)+(pow(y,2))),1.5)) - (GMm*(y))/(pow((pow((x-Rm),2)+(pow(y,2))),1.5));

   return ay;
}

double xinc(double x, double h, double k1x, double k2x, double k3x, double k4x)
{
    x+=(h*(k1x+2*k2x+2*k3x+k4x))/6.0;
    return x;
}

double yinc(double y, double h, double k1y, double k2y, double k3y, double k4y)
{
    y+=(h*(k1y+2*k2y+2*k3y+k4y))/6.0;
    return y;
}

double vxinc(double vx, double h, double k1vx, double k2vx, double k3vx, double k4vx)
{
   vx+=(h*(k1vx+2*k2vx+2*k3vx+k4vx))/6.0;
   return vx;
}

double vyinc(double vy, double h, double k1vy, double k2vy, double k3vy, double k4vy)
{
    vy+=(h*(k1vy+2*k2vy+2*k3vy+k4vy))/6.0;
    return vy;
}


