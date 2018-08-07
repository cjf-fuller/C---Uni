#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dfc (double h1, double x1);
double dff (double h1, double x1);
void filedata(double h1, double x1);
double df2 (double h1, double x1);

int main()
{
    double x ,h;
    printf("Please enter a value for x:\n");
    scanf("%lf", &x);
    do
    {
        printf("Please enter a value for h (h must be less than 1):\n");
        scanf("%lf", &h);
        if (h>=1)
        {
            printf("\n Error in length (must be non-zero and positive)\n");
        }
    }
    while (h>=1);
    filedata(h, x);
    return 0;
}

double dfc (double h1, double x1)//central function
{
    double dfc, fx, afxh, sfxh;
    fx = x1 * exp(x1);
    afxh = (x1+h1)*(exp(x1+h1));
    sfxh = (x1-h1)*(exp(x1-h1));
    dfc = (afxh - sfxh)/(2*h1);
    return dfc;
}

double dff(double h1, double x1)//forward function
{
    double dff, fx, afxh;
    fx = x1 * exp(x1);
    afxh = (x1+h1)*(exp(x1+h1));
    dff = (afxh - fx)/h1;
    return dff;
}

double df2(double h1, double x1)
{
     double df2, fx, afxh, sfxh;
     fx = x1 * exp(x1);
     afxh = (x1+h1)*(exp(x1+h1));
     sfxh = (x1-h1)*(exp(x1-h1));
     df2 = (afxh - 2*fx + sfxh)/(h1*h1);
     return df2;
}

void filedata (double h1, double x1)
{
    double i, errc, errf, err2;
    FILE *varx;
    FILE *varh;
    FILE *varh2;
    varx = fopen("vary_x_results.txt", "w");//Create a file to store data in.
    if (varx != NULL)
    {
        printf("File created with varied values of x (0<=x<=2) and constant h=0.0001\n");
        fprintf(varx, "Value of h = %lf \n\n Value of x  \t First Derivative (forward formulae) \t Error in forward formula \t\t First Derivative (central formulae) \t Error in central formula \n  ", h1);
    }
    else
    {
        printf("Un-able to create file\n");
    }
    for (i=0; i<=2; i+=0.001)//loop for diff x's
    {
        errf=fabs(dff(h1,i)-((exp(i))*(i+1)));
        errc=fabs(dfc(h1,i)-((exp(i))*(i+1)));
        fprintf(varx, "%0.10lf \t|\t %0.10lf \t\t|\t %0.10lf \t\t|\t\t %0.10lf \t\t|\t\t %0.10lf\n", i, dff(h1,i), errf, dfc(h1,i), errc);
        errf=0;
        errc=0;
    }
    printf("\nData successfully read into a file named 'vary_x_results.txt'\n ");


     varh = fopen("vary_h_results.txt", "w");//Create a file to store data in.//
    if (varh != NULL)
    {
        printf("File created with varied values of h (10^-15<=h<=0.1) with a constant x=1 \n");
        fprintf(varh, "Value of x = %0.12lf \n\n Value of h  \t\t First Derivative (forward formulae) \t\t Error in forward formula \t\t  First Derivative (central formulae) \t\t Error in central formula \n  ", x1);
    }
    else
    {
        printf("Un-able to create file\n");
    }

    for(h1=pow(10,-15); h1<=0.1; h1*=1.1)//loop for diff h's
    {
      errf=fabs(dff(h1,x1)-((exp(x1))*(x1+1)));
      errc=fabs(dfc(h1,x1)-((exp(x1))*(x1+1)));
      fprintf(varh, "%0.15lf \t|\t %0.15lf \t\t|\t %0.15lf \t\t|\t\t %0.15lf \t\t|\t\t %0.15lf\n", h1, dff(h1,x1), errf, dfc(h1,x1), errc );
      errf=0;
      errc=0;
    }
    printf("\nData successfully read into a file named 'vary_h_results.txt'\n");



    varh2 = fopen("vary_h_results_for_2nd_derivative.txt", "w");//Create a file to store data in.
    if (varh2 != NULL)
    {
        printf("File created with varied values of h (10^-15<=h<=0.1) with a constant x=1 for computing the SECOND derivative \n");
        fprintf(varh2, "Value of x = %0.12lf \n\n Value of h  \t\t Second Derivative \t\t Error in second derivative\n  ", x1);
    }
    else
    {
        printf("Un-able to create file\n");
    }

    for(h1=pow(10,-15); h1<=0.1; h1*=1.1)//loop for diff h's
    {
      err2=fabs(dff(h1,x1)-((exp(x1))*(x1+1)));
      fprintf(varh2, "%0.15lf \t|\t %0.15lf \t|\t %0.15lf\n", h1, df2(h1,x1), err2 );
      err2=0;

    }
    printf("\nData successfully read into a file named 'vary_h_results_for_2nd_derivative.txt'\n");
}
