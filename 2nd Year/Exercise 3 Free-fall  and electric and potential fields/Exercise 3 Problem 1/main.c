#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dragin();//Define input functions
double areain();
double massin();
double dtin();
double heightin();
double velin();
double terminal();
void eulercalc(double mg, double v0g, double y0g, double dtg, double Ag, double Cg, double vtg, double pg, double jg, double kg);//this is the main calculation function
double vn(double v0g, double dtg, double mg, double kg);
double ync(double y0g, double dtg, double v0g);
double yanalyt(double y0g, double mg, double kg, double tg);
double vanalyt(double mg, double kg, double tg);


int main()
{
    double C, A, m, v0, y0, dt, p, vt, k;
    int j;

    printf("Free-fall simulation with fixed or varying drag.\n\nValue of g=-9.81ms^-2\n\n");
    C = dragin();
    A = areain();
    m = massin();
    dt = dtin();
    y0 = heightin();
    v0 = velin();
    do
    {
        printf("For constant air density press 1\nFor varying air density press 2\n");
        scanf("%d", &j);
        if (j > 2)
        {
            printf("Error - Value MUST be 1 or 2");
        }
    }
    while (j > 2 || j < 1);
    if (j == 1)//if user has selected varyied density percentage is taken here and used as an argument
    {
    p = terminal();
    vt = (p/100)*sqrt((2*m*9.81)/(C*1.2*A));
    k = (C*A*1.2)/2;
    }

    eulercalc(m,v0,y0,dt,A,C,vt,p,j,k);
    printf("\n\nProgram complete - results have been written to a file named 'Free-fall-result.txt'");
    return 0;
}

double dragin()//this input method is repeated for each variable
{
    char input[256];
    double check;
    double drag;

    while (1)//(1) is put so loop repeats until correct cast is put
    {
        printf("Please enter the drag coefficient for the free-fall object:");
        fgets(input, sizeof input, stdin);//takes inputted value and converts to stream
        check = sscanf(input, "%lf", &drag);//check to see if stream is correct cast and assign appropriately
        if (check == 1) break;//if it is correct cast it will return 1 if not it will return 0 and the loop wont break
        printf("Error invalid input type\n\n");
    }
    return drag;
}

double areain()
{
    char input[256];
    double check;
    double area;

    while (1)
    {
        printf("Please enter the cross sectional area of the projectile (m^2):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &area);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    return area;
}

double massin()
{
    char input[256];
    double check;
    double mass;

    while (1)
    {
        printf("Please enter the mass for the free-fall object (kg):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &mass);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    return mass;
}

double dtin()
{
    char input[256];
    double check;
    double dt;

    while (1)
    {
        printf("Please enter the step size, dt, (seconds):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &dt);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    return dt;
}

void eulercalc(double mg, double v0g, double y0g, double dtg, double Ag, double Cg, double vtg, double pg, double jg, double kg)
{
    int i;
    double t, y0gn, y01, v01, tv;
    FILE *ffresults;

    ffresults = fopen("Free-fall-result.txt", "w");//openign file to store results
    if (ffresults != NULL)
    {
        printf("File created containing the data from the free-fall simulation\n");
        fprintf(ffresults, "Initial Displacement = %lf  Initial Velocity = %lf \n\nTime (seconds) \t Displacement (m) \t Analytical Displacement (m) \t Velocity (ms^-1) \t Analytic Velocity (ms^-1)\n", y0g, v0g);
    }
    else
    {
        printf("Un-able to create file\n");
    }
    t=0;//initialise time
    i=1;//set counter
    y0gn=y0g;//this is doen for correct analytical values
    printf("Computing Euler's method...\n");
    while (y0g >= 0)
    {
        if (jg ==2 )
        {
            kg = ((Cg*Ag*1.2)/2)*exp(-y0g/7460);//setting of constant density if user has requested
        }
        v01 = v0g;//previous velocity is stored as v01 to find impact time
        v0g = vn(v0g, dtg, mg, kg);//Euler calculation is performed
        y0g = ync(y0g, dtg, v0g);
        if (fabs(v0g)>vtg && jg==1 && i==1)//conditional statement to see if percentage of term vel. has been met (for the first time)
        {
            tv=t;
            i++;//increase counter count so tv isn't reset
        }

        t+=dtg;
        fprintf(ffresults, "%0.10lf \t\t %0.10lf \t\t %0.10lf \t\t %0.10lf \t\t %0.10lf \n", t, y0g, yanalyt(y0gn,mg,kg,t), v0g, vanalyt(mg,kg,t));

    }
    t-=(fabs(y0g))/((fabs(v0g+v01))/2);//impact time calculation

    fprintf(ffresults, "\n\nApproximate impact time was %lf seconds", t);
    printf("\nApproximate impact time was %lf seconds", t);

    if (jg==1 && i==2)
    {
        fprintf(ffresults, "\n\n\n It took %lf seconds to reach %lf% of terminal velocity.", tv, pg);
        printf("\n\n\n It took %lf seconds to reach %0.1lf percent of terminal velocity.", tv, pg);
    }

}

double vn(double v0g, double dtg, double mg, double kg)
{
    double vn;
    vn = v0g - dtg*(9.81+(kg/mg)*v0g*fabs(v0g));
    return vn;
}

double ync(double y0g, double dtg, double v0g)
{
    double yn;
    yn = y0g + dtg*v0g;
    return yn;
}

double yanalyt(double y0g, double mg, double kg, double tg)
{
    double yan;
    yan = y0g-(mg/(kg))*log(cosh(tg*sqrt(((kg*9.81)/mg))));
    return yan;
}

double vanalyt(double mg, double kg, double tg)
{
    double van;
    van = - (pow((mg*9.81)/kg,0.5))*tanh(tg*pow(((kg*9.81)/mg),0.5));
    return van;
}

double heightin()
{
    char input[256];
    double check;
    double height;

    while (1)
    {
        printf("Please enter the initial height of the jump (m):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &height);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    return height;
}

double velin()
{
    char input[256];
    double check;
    double vel;

    while (1)
    {
        printf("Please enter the initial velocity of the jump (ms^-1):");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &vel);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    return vel;

}
double terminal()
{
    char input[256];
    double check;
    double percent;

    while (1)
    {
        printf("Please enter the percentage of terminal velocity you want to find the time\nthe body takes to reach (only valid for constant air density):\n");
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &percent);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    return percent;

}
