#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>

void file();//define external functions
void calculation(FILE *fp2,int n);
int posint();

int main (void)
{

    int n, user;
    printf("Program to find the upper bound on a cross section using the Toy Monte Carlo method.\n");

    FILE* right = fopen("Poisson Results for the correct cross section.txt", "w");
    file();//checks on file openings

    printf("\n\nPlease press '1' to input the number of pseudo-experiments generated manually\n or '2' to use n=1,000,000 pseudo-experiments:");
    user = posint();//Handling user input to check right input format
    if(user==1)
    {
        printf("Please enter an integer number of pseudo-experiments to be generated:");
        n = posint();
        printf("\nSimulations will now be run for n=%d pseudo-experiments.\n",n);
    }
    else if(user == 2)
    {
        n = 1000000;
        printf("\nSimulations will now be run for n=1,000,000 pseudo-experiments.\n");
    }
    else
    {
        printf("Error - input must be 1 or 2\n\n");
        perror(1);
    }
    calculation(right,n);
    return 0;
}
void file()
{
    FILE *right;
    right = fopen("Poisson Results for the correct cross section.txt", "w");//Create a file to store data in.
    if (right != NULL)
    {
        printf("File created with Poisson Random Numbers for the correct cross section.\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }

}

void calculation(FILE *fp2, int n)
{
    int i, below5=0.0, counter=0.0;
    double mu = 5.8, sigma = 0.4, av=0.0, tav=0.0, jump=131072;//large jump value used to cover a broad range of cross sections
    double cs,conf,k,m,prob,l;
    const gsl_rng_type * typ;
    gsl_rng * rnd;
    gsl_rng_env_setup();
    typ = gsl_rng_default;//initialise the random number generator
    rnd = gsl_rng_alloc (typ);

    for (i = 0; i < n; i++)
    {
        k = gsl_ran_gaussian (rnd, sigma);
        av += (k+mu);//centre the gaussian distribution on 5.8 and sum for the overall average.
    }
    tav = av/n;//find the mean number of background events from a randomly generated gaussian distribution

    cs=0.0;
    do
    {
        for(i=0; i<n; i++)
        {
            l =10 + gsl_ran_gaussian (rnd, 0.15);//generate an luminosity value with an uncertainty.
            m = gsl_ran_poisson (rnd, (tav + l*cs));//generated random numbers in a poisson distribution

            counter++;//count for the number of pseudo experiments.
            if  (m<=5.0)
            {
                below5++;//count for the number of random poisson distributed numbers are below 5.

            }
        }
        prob = (double) below5/counter;//probability of a random poisson distributed number being below 5.

        conf = 1.0 - prob;//find the confidence - the probability of the random number being greater than 5.
        below5=0.0;
        counter = 0.0;
        if (conf<0.95)//convergence condition
        {
            cs += jump;//if confidenc level hasn't been reached it will jump again.
        }
        if (conf>0.95)
        {
            cs-=jump;//if confidence level has been reached it will jump back one step.
            jump/=2;//then half the the jump to converge on the true answer.
        }
    }
    while(jump>0.00000001); //convergence condition for how accurate the final result should be.
    cs-=jump;//routine will over overstepped by one jump so need to go back one for the true answer.
    printf("\n\nProcess complete! The upper bound on the cross section was found to be %.9lf m^2 for an accuracy of 0.00000001 at the 95 percent confidence level \n\n", cs);
    for(i=0; i<n; i++)
    {
        m = gsl_ran_poisson (rnd, (tav + l*cs));//print the correct value to the file.
        fprintf(fp2,"%lf\n",m);
    }

    gsl_rng_free (rnd);
}



int posint()
{
    char input[256];
    double check;
    int d=0;

    while (1)
    {
        fgets(input, sizeof input, stdin);

        check = sscanf(input, "%d", &d);//Check the input was the correct type
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    if(d<=0)//check the input was within bounds
    {
        printf("Error - n must be positive\n\n");
        perror(1);
    }

    return d;
}
