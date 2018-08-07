#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>//Use of gsl_math.h for a better M_PI value
#include <gsl/gsl_histogram.h>

void randdist(int n);//defining external functions here
int posint();

int main ()
{
    int i, n, user;
    printf("Program to generate a non-uniform random distribution of numbers proportional to sin(x)\nUsing analytical and reject-accept techniques.");
    printf("\n\nPlease press '1' to input the number of random numbers generated manually\n or '2' to use n=1,000,000 random numbers:");
    user = posint();//Handling user input to check right input format
    if(user==1)
    {
        printf("Please enter an integer number of random numbers to be generated:");
        n = posint();
    }
    else if(user == 2)
    {
        n = 1000000;
    }
    else
    {
        printf("Error - input must be 1 or 2\n\n");
        perror(1);
    }


    randdist(n);

    return 0;
}

void randdist(int n)
{
    int j;
    double u, v, w, y;
    const gsl_rng_type * Typ;
    gsl_rng * rnd;


    FILE *randan;
    randan = fopen("analytical results.txt", "w");//Create a file to store data in.
    if (randan != NULL)
    {
        printf("File created with results from the analytical method.\n");
        fprintf(randan, "RANDOM NUMBERS\n  ");
    }
    else
    {
        printf("Un-able to create file\n");
    }

    FILE *rejacc;
    rejacc = fopen("reject-accept results.txt", "w");//Create a file to store data in.
    if (rejacc != NULL)
    {
        printf("File created with results from reject-accept sampling.\n");
        fprintf(rejacc, "RANDOM NUMBERS\n  ");
    }
    else
    {
        printf("Un-able to create file\n");
    }

    gsl_rng_env_setup();

    Typ = gsl_rng_taus2;//Set up the gsl random number generator
    rnd = gsl_rng_alloc (Typ);

    for (j = 0; j < n; j++)
    {

        u = gsl_rng_uniform (rnd);//create two random numbers, one for each method.
        y = gsl_rng_uniform (rnd);
        w = u* M_PI;//scale the analytical methods random number.
        v = acos(1-(2*u));//perform the analytical transformation.
        fprintf(randan, "%lf\n", v);
        if(y > sin(w))//test for the reject-accept method to see if within bounds.
        {
            continue;//reject the value
        }
        else
        {
            fprintf(rejacc, "%lf\n", w);//accept the value
        }
    }

    gsl_rng_free (rnd);//free the memory space used.

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
