#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double NR(double a0g,double a1g,double a2g,double a3g,double a4g,double x0g,double accg);
double FX(double a0g,double a1g,double a2g,double a3g,double a4g,double x0g);
double FDX(double a1g,double a2g,double a3g,double a4g,double x0g);

int main()
{
    double a0, a1, a2, a3, a4, x0, acc, diff, x0n, xmin, xmax, incr, j,k;
    int i;
    FILE *NRresults;
    FILE *NRloop;

    printf("\nFinding the Roots of a general quartic\n\n");
    printf("Please enter values for constants a0, a1, a2, a3 and a4, separated by a space:\n");
    scanf("%lf %lf %lf %lf %lf", &a0, &a1, &a2, &a3, &a4);
    printf("Please enter an initial guess for x0:\n");
    scanf("%lf", &x0);
    printf("Please enter the accuracy required:\n");
    scanf("%lf", &acc);
    NRresults = fopen("Newton_Raphson_results.txt", "w");//Create a file to store data in.//
    if (NRresults != NULL)
    {
        printf("File created containing the results from the Newton Raphson method\n (roots and iteration number)\n");
        fprintf(NRresults, "Difference \t x(i+1) \t xi \t Iteration number\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    x0n=0;
    for(i=1; i<=100; i++)
    {
        x0n=x0-(FX(a0, a1, a2, a3, a4, x0))/(FDX(a1, a2, a3, a4, x0));
        diff=fabs(x0n-x0);
        fprintf(NRresults, "%0.10lf \t %lf \t %lf \t %d \n", diff, x0n, x0, i);
        x0=x0n;
        if (diff<=acc)
        {
            printf("One root of the polynomial is %lf and took %d iterations to find.\n", x0, i);
            fprintf(NRresults, "\n\n\nOne root of the polynomial is %lf and took %d iterations to find.\n", x0, i);
            break;
        }

    }
    if (diff>acc)
    {
        printf("Maximum number of iterations reached without finding root.");
    }

    printf("\n\nLooping Newton Raphson over many values of x to find roots of f(x)\n\n (f(x) is the same polynomial as used in part b)\n");
    printf("Please enter minimum value of x:\n");
    scanf("%lf", &xmin);
    printf("Please enter maximum value of x:\n");
    scanf("%lf", &xmax);
    printf("Please enter an increment for x to loop on between min and max values:\n");
    scanf("%lf", &incr);
    a0=10;
    a1=-2;
    a2=-12;
    a3=1;
    a4=1;
    NRloop = fopen("Newton_Raphson_loop_results.txt", "w");//Create a file to store data in.//
    if (NRloop != NULL)
    {
        printf("File created containing the results from the Newton Raphson method\n (roots and iteration number)\n");
        fprintf(NRloop, "Value of xo \t f(x) \t Value of root \t Number of iterations to converge\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }

    for (k=xmin; k<=xmax; k+=incr)
    {
        fprintf(NRloop, "%lf \t %lf", k, FX(a0, a1, a2, a3, a4, k));
        j=k;
        for(i=1; i<=100; i++)
        {
            x0n=j-(FX(a0, a1, a2, a3, a4, j))/(FDX(a1, a2, a3, a4, j));
            diff=fabs(x0n-j);
            j=x0n;
            if (diff<=0.000001)
            {
                fprintf(NRloop, "\t %lf \t %d \n", j, i);
                break;
            }
        }

    }
printf("Results have been written to a .txt file");

    return 0;
}
double FX(double a0g,double a1g,double a2g,double a3g,double a4g,double x0g)
{
    double FX;
    FX=a0g+(a1g*x0g)+(a2g*(pow(x0g,2)))+(a3g*(pow(x0g,3)))+(a4g*(pow(x0g,4)));
    return FX;
}
double FDX(double a1g,double a2g,double a3g,double a4g,double x0g)
{
    double FDX;
    FDX=a1g+(2*a2g*x0g)+(3*a3g*(pow(x0g,2)))+(4*a4g*(pow(x0g,3)));
    return FDX;
}


