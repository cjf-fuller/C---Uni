#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double vin();//prototype functions declared here
double rin();
double tin();
double angin();
double axc(double x, double y);
double ayc(double x, double y);
double xinc(double x, double h, double k1x, double k2x, double k3x, double k4x);
double yinc(double y, double h, double k1y, double k2y, double k3y, double k4y);
double vxinc(double vx, double h, double k1vx, double k2vx, double k3vx, double k4vx);
double vyinc(double vy, double h, double k1vy, double k2vy, double k3vy, double k4vy);
double tinc(double t, double h);

int main()
{
    double x,y,vx,vy,v,r,a,t,h,k1x,k1y,k1vx,k1vy,k2x,k2y,k2vx,k2vy,k3x,k3y,k3vx,k3vy,k4x,k4y,k4vx,k4vy, t_end, r0, re, E, GMe, U, KE;
    v=vin();//user input prompts for initial conditions
    r=rin();
    t_end=tin();
    a=(M_PI/180)*angin();//converting the angle to radians
    x=r*cos(a);//use of polar coordinate trigonometry to find initial conditiosns for the Runge-Kutta method
    y=r*sin(a);
    vy=v*-cos(a);
    vx=v*sin(a);
    h=1;//model becomes innacurate at greater h's making a user inputted value unnessecary
    re=6371000;
    GMe = 3.9853545e+14;
    FILE *orbitcalc;

    orbitcalc=fopen("OrbitCalc.txt","w");//open file to store data
     if (orbitcalc != NULL)
    {
        printf("File created containing the data from the orbit simulation\n");
        fprintf(orbitcalc, "Initial tangential velocity: %lf ms^-1 \t Radius of Orbit: %lf (m) \n\n\n\t Time (s) \t x (m) \t y (m) \t vx (ms^-1) \t vy (ms^-1) \t Energy (J) \t Potential Energy (J) \t Kinetic Energy (J) \n", v,r);
    }
    else
    {
        printf("Un-able to create file\n");
    }

   t_end *= 24*60*60;//converts time from days to seconds
   t=0;
    do
    {
    k1x = vx;//application of the Runge-Kutta method
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
    r0 = pow(x*x + y*y,0.5);//calculation of orbital radius
    v = pow(vx*vx +vy*vy,0.5);//calculation of velocity to find kinetic energy
    E = - (GMe/r0) + (0.5*pow(v,2));//assuming mass=1kg
    U = - (GMe/r0);
    KE = 0.5*pow(v,2);
    t++;
    if (r0<=re)//check to see if satellite has crashed into earth
    {
        printf("The satellite has crashed into earth!");
        break;
    }
    printf("%lf %lf %lf %lf %lf %lf %lf %lf \n", x, y, vx, vy, t, E, U, KE);
    fprintf(orbitcalc,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%le\t%lf\n", t, x, y, vx, vy, E, U, KE);
    }while (t<=t_end);
    printf("\n\nProcess complete! Data written to OrbitCalc.txt");
    return 0;
}


double vin()//user prompted input
{
    char input[256];
    double check;
    double vel;


    while (1)
    {
        printf("\n\nPlease enter the initial orbital velocity of the\n small orbitting body (ms^-1):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &vel);//check to ensure correct type is inputted
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }

    return vel;
}
double rin()
{
    char input[256];
    double check;
    double rad;

    do{
    while (1)
    {
        printf("\n\nPlease enter the initial distance between the bodies (m):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &rad);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    if(rad<=0)//check to ensure value is an allowed value
    {
        printf("Error - Distance must be positive\n\n");
    }}while(rad<=0);
    return rad;
}

double tin()
{
    char input[256];
    double check;
    double t;

    do{
    while (1)
    {
        printf("\n\nPlease enter the time you want to track the orbit for (in days):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &t);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    if(t<=0)
    {
        printf("Error - Time must be positive\n\n");
    }}while(t<=0);
    return t;
}


double angin()
{
    char input[256];
    double check;
    double ang;

    do{
    while (1)
    {
        printf("\n\nPlease enter the angle (anti-clockwise from the x-axis) the satellite begins orbiting at (in degrees 0-360):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &ang);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
   if (ang<0 || ang >360)
   {
    printf("Error - Angle must be within limits (0-360)\n\n");
    }}while(ang<0 || ang >360);
    return ang;
}

double axc(double x, double y)
{
   double ax, GMe;
   GMe = 3.9853545e+14;
   ax = -(GMe*x)/(pow((pow(x,2)+(pow(y,2))),1.5));
   return ax;
}

double ayc(double x, double y)
{

   double ay, GMe;
   GMe = 3.9853545e+14;
   ay = -(GMe*y)/(pow((pow(x,2)+(pow(y,2))),1.5));

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


