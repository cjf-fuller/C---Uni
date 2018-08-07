#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_matrix.h>//Required libraries included for matrix manipulation and LU Decomposition
#include <gsl/gsl_linalg.h>


int main()
{
    int signum,i,j, check, time=0;
    double k = 0.59;
    double den= 0.0079;
    double cp = 450.0;

    double a,b,pre;
    double change;
    double acc;
    FILE *heat;
    FILE *iceheat;
    double alpha = k/(cp*den);

    printf("SOME STUFF ABOUT HEAT AND STUFF.\n\n");
    heat = fopen("heat-results.txt", "w");
    if (heat != NULL)
    {
        printf("File created containing the data for the temperature\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    iceheat = fopen("iceheat-results.txt", "w");
    if (iceheat != NULL)
    {
        printf("File created containing the data for the temperature for the ice problem\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }

    a = -alpha*100;
    b = 1+(2*alpha*100);
    acc=0.0001;


    gsl_matrix *cons = gsl_matrix_alloc (50,50);//Allocating the memory for the matrices and permutations required
    gsl_matrix *ice = gsl_matrix_alloc (50,50);
    gsl_vector *tdash = gsl_vector_alloc (50);
    gsl_vector *tdashice = gsl_vector_alloc (50);
    gsl_vector *t = gsl_vector_alloc (50);
    gsl_vector *tice = gsl_vector_alloc (50);
    gsl_permutation *p = gsl_permutation_alloc (50);

    for(i=0;i<50;i++)
    {
        gsl_vector_set(t,i,293.15);
        gsl_vector_set(tice,i,293.15);
    }
    gsl_vector_set(t,49,1273.15);
    gsl_vector_set(tice,49,1273.15);
    gsl_vector_set(tice,0,273.150);

	for(i=0;i<50;i++)
    {
       for(j=0;j<50;j++)
    {
        if(i==j)
        {
            gsl_matrix_set(cons,i,j,b);
            gsl_matrix_set(ice,i,j,b);
        }
        else if(i+1==j)
        {
            gsl_matrix_set(cons,i,j,a);
            gsl_matrix_set(ice,i,j,a);
        }
        else if(i-1==j)
        {
            gsl_matrix_set(cons,i,j,a);
            gsl_matrix_set(ice,i,j,a);
        }
        else
        {
            gsl_matrix_set(cons,i,j,0);
            gsl_matrix_set(ice,i,j,0);
        }
    }
    }
   gsl_matrix_set(cons,0,0,(1+(alpha*100)));
   gsl_matrix_set(ice,0,0,1);
   gsl_matrix_set(cons,0,1,a);
   gsl_matrix_set(ice,0,1,0);
   gsl_matrix_set(cons,49,48,0);
   gsl_matrix_set(ice,49,48,0);
   gsl_matrix_set(cons,49,49,1);
   gsl_matrix_set(ice,49,49,1);

   gsl_linalg_LU_decomp(cons, p ,&signum);

   gsl_linalg_LU_decomp(ice, p ,&signum);



   do
   {

       check=0;

       gsl_linalg_LU_solve (cons,p,t,tdash);

       for (i=0;i<50;i++)
       {

        change = (fabs(gsl_vector_get(tdash,i)-gsl_vector_get(t,i)))/gsl_vector_get(tdash,i);

        if(acc<=change)
        {
       check++;

        }
        gsl_vector_set(t,i,gsl_vector_get(tdash,i));



        }
       time+=100;


     /*for(i=0;i<50;i++)
      {
       fprintf(heat, "%d \t %d \t %g \n", time, i, gsl_vector_get(t,i));
      }*/

   }while(check != 0.0);
   for(i=0;i<50;i++)
      {
       fprintf(heat, "%d \t %d \t %g \n", time, i, gsl_vector_get(t,i));
      }
time=0;
do
   {

       check=0;

       gsl_linalg_LU_solve (ice,p,tice,tdashice);

       for (i=0;i<50;i++)
       {

        change = (fabs(gsl_vector_get(tdashice,i)-gsl_vector_get(tice,i)))/gsl_vector_get(tdashice,i);
        if(acc<=change)
        {
       check++;
        }
        gsl_vector_set(tice,i,gsl_vector_get(tdashice,i));
        }
       time+=100;

       /*for(i=0;i<50;i++)
       {
       fprintf(iceheat, "%d \t %d \t %g \n", time, i, gsl_vector_get(tice,i));
       }
*/

   }while(check != 0.0);

for(i=0;i<50;i++)
       {
       fprintf(iceheat, "%d \t %d \t %g \n", time, i, gsl_vector_get(tice,i));
       }



printf("process returned!" );


    return 0;
}
