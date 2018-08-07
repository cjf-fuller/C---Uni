#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>

void file();
void squaredecaycalc(FILE *fp1, FILE *fp2, FILE *fp3,int n);
void spheredecaycalc(FILE *fp4,int n);
int posint();

int main ()
{
    int n,user;

    printf("Program to simulate the decay of unstable nuclei and the resulting gamma ray distribution.\n\n");
    FILE *expd = fopen("exponential distribution.txt", "w");
    FILE *sgamma = fopen("simple gamma ray distribution.txt", "w");
    FILE *gamma = fopen("smeared gamma ray distribution.txt", "w");
    FILE *sphgamma = fopen("spherical detector gamma ray distribution", "w");
    file();

    printf("\n\nPlease press '1' to input the number of nuclei sample generated manually\n or '2' to use n=1,000,000 nuclei:");
    user = posint();//Handling user input to check right input format
    if(user==1)
    {
        printf("Please enter an integer number of nuclei to be generated:");
        n = posint();
        printf("\nSimulations will now be run for n=%d nuclei for a square and spherical detector.\n",n);
    }
    else if(user == 2)
    {
        n = 1000000;
        printf("\nSimulations will now be run for n=1,000,000 nuclei for a square and spherical detector.\n");
    }
    else
    {
        printf("Error - input must be 1 or 2\n\n");
        perror(1);
    }
    squaredecaycalc(expd, sgamma, gamma, n);
    spheredecaycalc(sphgamma, n);

    return 0;
}

void file()
{
    FILE *expd;
    expd = fopen("exponential distribution.txt", "w");//Create a file to store data in.
    if (expd != NULL)
    {
        printf("File created with exponential distribution of random numbers.\n");
        fprintf(expd, "RANDOM NUMBERS\n  ");
    }
    else
    {
        printf("Un-able to create file\n");
    }

    FILE *sgamma;
    sgamma = fopen("simple gamma ray distribution.txt", "w");//Create a file to store data in.
    if (sgamma != NULL)
    {
        printf("File created with distribution of simple gamma ray model.\n");
        fprintf(sgamma, "X \t Y \n");
    }
    else
    {
        printf("Un-able to create file\n");
    }

    FILE *gamma;
    gamma = fopen("smeared gamma ray distribution.txt", "w");//Create a file to store data in.
    if (gamma != NULL)
    {
        printf("File created with distribution of smeared gamma ray model.\n");
        fprintf(gamma, "X \t Y \n");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    FILE *sphgamma;
    sphgamma = fopen("spherical detector gamma ray distribution.txt", "w");//Create a file to store data in.
    if (sphgamma != NULL)
    {
        printf("File created with distribution of gamma ray model for a spherical detector of radius 1m.\n");
        printf("The injection point is at x=y=z=0");
        fprintf(sphgamma, "X \t Y \t Z\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }


}

void squaredecaycalc(FILE *fp1, FILE *fp2, FILE *fp3,int n)
{
    const gsl_rng_type * typ;
    gsl_rng * rnd;
    int i;
    double mu = 520E-6;//mean lifetime
    double theta, phi;
    double x,y,z,smearx,smeary,k,v;

    gsl_rng_env_setup();
    typ = gsl_rng_taus2;//Set up the gsl random number generator
    rnd = gsl_rng_alloc (typ);
    for (i = 0; i < n; i++)
    {
        k = (gsl_ran_exponential (rnd, mu)*2000);//generate random decay lengths that follow the exponential decay law.

        fprintf (fp1, "%lf\n", k);
        if(k>2)
        {
            continue;//ignore decay lengths that pass the detector.
        }

        phi = (gsl_rng_uniform (rnd))*2*M_PI;//generate a random azimuthal angle

        v = gsl_rng_uniform (rnd);
        theta = acos(1-(2*v));//generate a random zenith angle with a sinusoidal distribution.
        if(theta>=((M_PI)/2))
        {
            continue;//ignore gamma rays that wont hit the detector
        }

        z=2.0-k;//find distance between decay event position and the detector.
        x = -z*tan(theta)*cos(phi);//find x,y coordinates of gamma ray when in line with the detector.
        y = z * tan(theta)*sin(phi);

        if(fabs(y)>1.5)//ignore rays that do not hit the detector
        {
            continue;
        }
        if(fabs(x)>1.5)
        {
            continue;
        }



        fprintf(fp2, "%lf \t %lf \n", x,y);
        smearx =  gsl_ran_gaussian (rnd, 0.1);//generate the random gaussian smear correction.
        smeary =  gsl_ran_gaussian (rnd, 0.3);
        fprintf(fp3,"%lf \t %lf \n", x+smearx, y+smeary);
    }


    gsl_rng_free (rnd);
}

void spheredecaycalc(FILE *fp4,int n)
{
    const gsl_rng_type * typ;
    gsl_rng * rnd;
    int i;
    double mu = 520E-6;//mean lifetime
    double theta, phi;
    double x,y,z,k,v,theta1,dist,c,b;

    gsl_rng_env_setup();
    typ = gsl_rng_taus2;//Set up the gsl random number generator
    rnd = gsl_rng_alloc (typ);
    for (i = 0; i < n; i++)
    {
        k = (gsl_ran_exponential (rnd, mu)*2000);//generate random decay lengths that follow the exponential decay law.

        if(k>2)
        {
            continue;//ignore decay lengths that pass the detector.
        }

        phi = (gsl_rng_uniform (rnd))*2*M_PI;//generate a random azimuthal angle

        v = gsl_rng_uniform (rnd);
        theta = acos(1-(2*v));//generate a random zenith angle with a sinusoidal distribution.

        theta1= theta;//create a dummy theta to be used in the cosine calculations.
        dist = 1-k;//find the distance from the centre of the sphere.

        if (k>1.0)//if the nuclei is past the midpoint of the sphere some corrections must be made.
        {
            theta1 = M_PI - theta;//change theta to ensure the right angle is used in the cosine rule
            dist = k-1;//ensure the right distance from the centre is found.

        }

        c = - (2 * dist * cos(theta1));//constant to be used in the quadratic

        b = ((-c)+pow(((pow(c,2))-(4*(pow(dist,2)-1))),0.5))/2;//solve the quadratic found from the cosine rule - taking the positive result.
//b corresponda to the distance from the decay event to the edge of a cross-section of the sphere.


        z = k + b*cos(theta);//find the new z-coordinate

        x = b*sin(theta)*cos(phi);//find the x,y coordinates to find where the ray hits the sphere
        y = b*sin(theta)*sin(phi);

        fprintf(fp4,"%lf \t %lf \t %lf\n", x, y, z);
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
