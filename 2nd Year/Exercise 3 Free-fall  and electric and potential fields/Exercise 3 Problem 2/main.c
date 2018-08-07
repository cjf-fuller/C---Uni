#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int main()
{

    double vold [32][17], vnew[32][17];
    double diff, ex, ey ,e, ang;
    int i, j, h, k, n;
    FILE *relax;


    printf("Relaxation Method for electric potential in a 2-core wire cross-section.\n\n");
    relax = fopen("Relaxation-results.txt", "w");
    if (relax != NULL)
    {
        printf("File created containing the data from the relaxation method\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }

    h=1;
    k=1;//set iteration number counter

    for (i=0; i<32; i++)
    {
        for(j=0; j<17; j++)
        {
            vold [i][j]= 0;//initialise both arrays
            vnew [i][j]=0;
        }
    }

    for (i=7; i<10; i++)
    {
        for(j=7; j<10; j++)
        {
            vold [i][j]=-10;//set the constant potentials in both areas and both arrays
            vnew [i][j]=-10;
            vold [i+15][j]=10;
            vnew [i+15][j]=10;
        }
    }

    do
    {
        n=0;//reset of difference counter
        for (i=1; i<31; i++)
        {
            for(j=1; j<16; j++)
            {
                vnew[i][j] = 0.25*((vold[i+h][j])+(vold[i-h][j])+(vold[i][j+h])+(vold[i][j-h]));//relaxation method
                if ((fabs((vnew[i][j])-(vold[i][j]))) !=0 && (fabs(vold[i][j])!=10))
                {
                    diff = fabs((vnew[i][j])-(vold[i][j]));
                    if (diff > 0.001)
                    {
                        n++;//condition statement to increase count if difference is greater than 0.001
                    }
                }

            }
        }

        for (i=1; i<31; i++)
        {
            for (j=1; j<16; j++)
            {
                vold [i][j] = vnew [i][j];//filling of 2nd array
            }
        }
        for (i=7; i<10; i++)
        {
            for(j=7; j<10; j++)
            {
                vold [i][j]=-10;//reseting core potentials
                vold [i+15][j]=10;
            }
        }

        fprintf(relax, "ITERATION NUMBER :%d\n", k);
        k++;
        for (j=0; j<17; j++)
        {
            for(i=0; i<32; i++)
            {
                fprintf(relax, "%0.2lf", vold[i][j]);
            }
            fprintf(relax, "\n");//formatting point to give array like output
        }
        fprintf(relax, "\n");
    }

    while (n != 0);
    fprintf(relax, "\n\nELECTRIC POTENTIAL AT FINAL ITERATION\n\nEx \t Ey \t E \t Angle\n");
    for (i=1; i<31; i++)
    {
        for(j=1; j<16; j++)
        {
            ex = -(vold[i+1][j]-vold[i-1][j])/2;//electric field components calculatins
            ey = -(vold[i][j+1]-vold[i][j-1])/2;
            e = sqrt((pow(ex,2))+(pow(ey,2)));
            if(ex<0)
            {
                ang=atan(ey/ex)+M_PI;
            }
            else
            {
                ang=atan(ey/ex);
            }
            fprintf(relax,"%lf \t %lf \t %lf \t %lf \t %d \t %d\n", ex, ey, e, ang, i, j);
        }
    }
    printf("\n\nProcess complete! All results successfully printed to text file named \n'Relaxation-results.txt'");
    return 0;
}

