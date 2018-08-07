#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double FX (int p0, double t0, double l0);
double smallapprox (double l0);
void simpscons(double l0, double t0);
void simpsvary(double l0);

int main()
{
    double l, t;
    printf("Program to carry out numerical integration \nvia Simpson's Rule for the time period of a simple pendulum\n\n");
    do//Enter value for length of pendulum with conditions.
    {
        printf("Please enter the length of the pendulum in metres:\n");
        scanf("%lf", &l);
        if (l<=0)
        {
            printf("\n Error in length (must be non-zero and positive)\n");
        }
    }
    while (l<=0);
    do //enter value for initial angle or choose to switch to part b) of problem.
    {
        printf("Please enter a value for the initial angle of oscillation (Angle<Pi(radians))\nOr enter '1234' to vary this angle between zero and Pi for various N values\n");
        scanf("%lf", &t);
        if(t==1234)// carry out part b (confused ordering to allow the exit to take effect if need be.)
        {
            simpsvary(l);
            return 0;//Note the secondary exit from function so as not to perform both operations.
        }
        else if(t>M_PI)
        {
            printf("Error in input angle - please check the bounds.\n");
        }
    }
    while (t>M_PI);
    simpscons(l, t);//part A
    return 0;
}

double FX (int p0, double t0, double l0)
{
    double g, FX;
    g=9.81;//define g here
    FX=4*(sqrt(l0/g))*(1/(sqrt(1-(pow(sin(t0/2),2))*(pow(sin(p0),2)))));//p0 is referred to as N value later
    return FX;

}

void simpscons(double l0, double t0)
{
    double h, oddsum, evensum, fullsum, f0, fN;
    long int N, n;

    do //Enter N value with conditions
    {
        printf("Please enter the number of approximations required (Must be an even value)\n");
        scanf("%d", &N);
        if (N % 2 != 0)
        {
            printf("Error in input - please check N is even\n");
        }
    }
    while (N % 2 != 0);

    h=(M_PI)/(2*N);
    oddsum=0;//initialise
    evensum=0;
    fullsum=0;
    for (n=1; n<N; n++)//loop and sum dependent on odd/even
    {
        if(n % 2 != 0)
        {
            oddsum += 4*FX(n,t0,l0);//carry out simpsons rule
        }
        else
        {
            evensum += 2*FX(n,t0,l0);
        }
    }
    f0 = 4*(sqrt(l0/9.81)) ;//can be seen from formula
    fN = FX(N,t0,l0); // these are the values not included in the loop
    fullsum= (h/3)*(oddsum + evensum + f0 + fN);//compute full sum
    printf("The approximation for the period of the pendulum of length %lf metres and initial angle %lf radians, was found to be %lf seconds.\n\n %d Approximation points were used to find this. ",l0, t0, fullsum, N);
}
void simpsvary(double l0)
{
    long int i,n, N;
    double j, oddsum, evensum, f0, fN, fullsum, h, diff;
    FILE *fp;//file pointer set


    fp = fopen("vary_theta_N_results.txt", "w");//Create a file to store data in.
    if (fp != NULL)
    {
        printf("File created with varied values of Theta (between 0 and Pi) and N (between 10 and 10,000)\n\n");
        fprintf(fp, "Value of Theta \t\t Value of N  \t Final Value of T \n  ");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    for (N=10; N<=10000; N*=10)
    {
        for(j=0; j<M_PI; j+=(M_PI)/1000)
        {
            h=(M_PI)/(2*N);
            oddsum=0;//initialise
            evensum=0;
            fullsum=0;
            for (n=1; n<N; n++)//loop and sum dependent on odd/even
            {
                if(n % 2 != 0)
                {
                    oddsum += 4*FX(n,j,l0);
                }
                else
                {
                    evensum += 2*FX(n,j,l0);
                }
            }
            f0 = 4*(sqrt(l0/9.81)) ;//can be see from formula
            fN = FX(N,j,l0); // these are the values not included in the loop
            fullsum= (h/3)*(oddsum + evensum + f0 + fN);
            fprintf(fp,"%lf \t\t %d \t\t %0.12lf \t\n", j, N, fullsum);

        }
    }
    printf("Results successfully read to file named 'vary_theta_N_results.txt'");

}
