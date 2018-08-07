#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<float.h>

double xinput();//Prototype functions declared//
double Ninput();
double arctanaprx(double xgen, double Ngen);
void filedata(double xgen, double Ngen);//void type used here as the function is not returning anything - just creating files//
void piaprx7();
void piaprx12();

int main()
{
    double x;
    double N;
    double Approximation;
    double m;

    x = xinput();
    N = Ninput();
    Approximation = arctanaprx( x, N);
    printf("\n\nx=%.10lf        N=%lf\n",x,N);
    printf("arctan approximation = %.10lf\n\n", Approximation);
    filedata(x,N);
    piaprx7();
    piaprx12();
    return 0;
}
double xinput()//Function for the user input for the argument of x//
{
    double x;
    do//Using a do while loop so program doesn't stop if the input value is invalid//
    {
        printf("Please input x (within bounds -1<=x<=1):\n");
        scanf("%lf", &x);
        if(fabs(x)> 1)//Condition given for x//
        {
            printf("\n Error in input value - check bounds of x.\n");
        }

    }
    while(fabs(x)> 1);

    return x;
}

double Ninput()//Function for the user input for the argument of N//
{
    double N;
    do//usign a do while loop so program doesn't stop if the input value is invalid//
    {
        printf ("Please input N (N must be greater than 0):\n");
        scanf("%lf", &N);
        if(N<1)//COndition given for N//
        {
            printf("Error in input value - check bounds of N.\n");
        }
    }
    while(N<1);
    return N;
}


double arctanaprx(double xgen, double Ngen)//Function for approximating arctanx//
{
    double i;
    double arctanaprx=0;
    for(i=0; i<=Ngen; ++i) //loop to find the sum of the series//
    {
        arctanaprx += ((pow((-1),i))/((2*i)+1))*(pow(xgen,((2*i)+1)));
    }
    return arctanaprx;
}

void filedata(double xgen, double Ngen)
{
    //varying x//
    double j;
    double k;
    double diff;
    double n;
    FILE *xresults;
    FILE *Nresults;
    FILE *Nsf;
    xresults = fopen("vary_x_results.txt", "w");//Create a file to store data in.//
    if (xresults != NULL)
    {
        printf("File created with varied values of x with user defined N\n");
        fprintf(xresults, "Value of x  \t True Value of arctanx \t  Approximate value of arctanx \t  Difference\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    for (j=-1; j<=1.01; j+=0.01) //1.01 used here to get results up to 1//
    {
        fprintf(xresults, "%0.2lf \t\t %0.10lf \t\t %0.10lf \t\t %0.10lf\n", j, atan(j), arctanaprx(j,Ngen), fabs(atan(j)-arctanaprx(j,Ngen)));
    }
    //varying N//
    Nresults = fopen("vary_N_results.txt", "w");//Create a file to store data in.//
    if (Nresults != NULL)
    {
        printf("File created with varied values of N (1 to 500) with user defined x\n");
        fprintf(Nresults, "Value of N  \t True Value of arctanx \t  Approximate value of arctanx \t  Difference\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    for (j=1; j<=500; j++) //Set variation of N between 1 and 500
    {
        fprintf(Nresults, "%0.2lf \t\t %0.10lf \t\t %0.10lf \t\t %0.10lf\n", j, atan(xgen), arctanaprx(xgen,j), fabs(atan(xgen)-arctanaprx(xgen,j)));
    }
    //varying x and N to 7s.f//
    Nsf = fopen("Nsfresults.txt", "w");
    if (Nsf != NULL)
    {
        printf("File created with results for finding N to give 7sf accuracy\n There are many iterations to perform so this step may take a while \n");
        fprintf(Nsf, "Value of N  \t\t Value of x \n");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    for (j=-1; j<=1.01; j+=0.01)//found to evaluate all except 1 and -1//
    {
        for(k=1; k<=1000; k++) //Nested loop//
        {
            diff=fabs(atan(j)-arctanaprx(j,k));
            if (diff <= 0.0000001 )
            {
                fprintf(Nsf, "%0.1lf \t\t %0.2lf\n", k,j);
                break;
            }
        }
    }
    j=1;//find N for specific x=1 and therefore x=-1 as odd function with abs(x)
    k=0;//intialise this value so it is not randomly assigned
    n=131072;//Chosen as divides by 2 till 1 is reached
    while(n!=1 || diff > 0.0000001)//should be && but crashes program
    {
        k+=n;
        diff =fabs(atan(j)-arctanaprx(j,k));

        if (diff < 0.0000001)
        {
            k-=n;
            n/=2;
        }
    }
    fprintf(Nsf, "%0.1lf \t %0.2lf\n", k,j);
    fprintf(Nsf, "%0.1lf \t -1.00\n", k);

}

void piaprx7()
{
    printf("\nEvaluating Pi to 7s.f by expanding 4*arctan(1) - This process also takes some time\n\n");
 double j;
 double k;
 double n;
 double diff;
 double piaprx;
 j=1;
 k=0;
 n=524288;
 while(diff > 0.000000153 || n!=1)
    {
        k=k+n;
        diff =fabs((M_PI)-(4*arctanaprx(j,k)));

        if (diff < 0.000000153)
        {
            k-=n;
            n/=2;
        }
    }
  printf("Aproximate of Pi to 7s.f - %lf took %lf iterations to reach\n", 4*arctanaprx(j,k), k );

}

void piaprx12()
{
    printf("\nEvaluating Pi to 12s.f by calling the taylor expansion 3 times\n\n");
    double i,j,k,n, diff;
    i=0.5;
    j=0.2;
    k=0.125;
    n=1;
    while(diff>0.00000000000153)
    {
        diff=fabs((M_PI)-(4*(arctanaprx(i,n)+arctanaprx(j,n)+arctanaprx(k,n))));
        n++;


    }

printf("Aproximate of Pi to 12s.f - %0.12lf took %lf iterations to reach\n",(4*(arctanaprx(i,n)+arctanaprx(j,n)+arctanaprx(k,n))), n);
}

