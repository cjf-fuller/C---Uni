#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int posint();
double dub();
double posdub();


int main()
{
    double **m=NULL, **u=NULL;
     int **ch=NULL;
    int j,r=0,c=0,i,check,it=0,xsize,ysize,xpos,ypos;
    double  T, B, L, R,change,acc,gues, charge;
    printf("\n\nPlease enter the Y length of the grid:");
    double row=posdub() +2;
    printf("\n\nPlease enter the X length of the grid:");
    double col=posdub() +2;
    printf("\n\nPlease enter the step size between nodes:");
    double h = posdub();
    xsize = (int)col/h;
    ysize = (int)row/h;
    FILE *pot;


    printf("SOME STUFF ABOUT ELECTRIC AND STUFF.\n\n");
    pot = fopen("1-potential-results.txt", "w");
    if (pot != NULL)
    {
        printf("File created containing the data for the final potential\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }

   m = (int**)malloc(ysize*(sizeof(double*)));
     u = (int**)malloc(ysize*(sizeof(double*)));
     ch = (int**)malloc(ysize*(sizeof(int*)));
    for (j=0; j<ysize; j++)
    {
        m[j] = (int*)malloc(xsize * sizeof(double));
        u[j] = (int*)malloc(xsize * sizeof(double));
        ch[j] = (int*)malloc(xsize * sizeof(int));
    }
    for(j=0; j<xsize; j++)
    {
        for(i=0; i<ysize; i++)
        {
            ch[i][j] = 0;
        }
    }
 /*  printf("\n\nPlease enter the initial value along the top of the array:");
 T=dub();
   printf("\n\nPlease enter the initial value along the bottom of the array:");
 B=dub();
   printf("\n\nPlease enter the initial value along the left of the array:");
 L=dub();
   printf("\n\nPlease enter the initial value along the right of the array:");
 R=dub();
   printf("\n\nPlease enter charge of point charge(if no point charge required enter zero):");
 charge=dub();
 if(charge != 0.0)
 {
     do{
        printf("\n\nPlease enter the x-position of the point charge:");
     xpos = (int) (posint())/h;
     if(xpos<xsize)
     {
         break;
     }
     printf("Error. Please enter a position that fits on the grid.\n");
     }while(1);

        do{
        printf("\n\nPlease enter the y-position of the point charge:");
     ypos = (int) (posint())/h;
     if(ypos<ysize)
     {
         break;
     }
     printf("Error. Please enter a position that fits on the grid.\n");
     }while(1);
 }*/
 charge=10;
 T=0;
 B=0;
 L=0;
 R=0;
 xpos = (int) 5/h;
ypos = (int) 5/h;
 //printf("\n\nPlease enter the percentage change convergence condition you require:");
 //acc=posdub();
 acc=0.0000001;
 printf("\n\nPlease enter the initial guess for the grid elements to take:");
 gues = dub();

    for(i=0; i<ysize; i++)
    {
        m[i][0]=L;
        m[i][xsize-1]=R;

        u[i][0]=L;
        u[i][xsize-1]=R;
    }

    for(j=0;j<xsize;j++)
    {
        m[0][j]=T;
        m[ysize-1][j]=B;
        u[0][j]=T;
        u[ysize-1][j]=B;
    }

//for(gues=-50;gues<200;gues+=10)
//{
//it=0;
    for(j=1; j<xsize-1; j++)
    {
        for(i=1; i<ysize-1; i++)
        {
            m[i][j] = gues;
            u[i][j] = gues;
        }
    }

    if(charge != 0.0)
    {
    m[ypos][xpos] = charge;
    //m[ypos+1][xpos+1] = -charge;
    u[ypos][xpos] = charge;
    //u[ypos+1][xpos+1] = -charge;
    ch[ypos][xpos] = 1;
    //ch[ypos+1][xpos+1] = 1;
    }
    //for(acc=100;acc>pow(10,-18);acc/=10)
    //{it=0;

    do
    {
        check=0;

        for(j=1; j<xsize-1; j++)
        {
            for(i=1; i<ysize-1; i++)
            {
                 if (ch[i][j]==1)
               {
                   continue;
               }

               u[i][j] = 0.25*((m[i+1][j])+(m[i-1][j])+(m[i][j+1])+(m[i][j-1]));


               if(u[i][j]==0.0 && m[i][j]==0.0)
               {
                 change = 0;
               }
               else
               {
                 change  = (fabs((u[i][j])-(m[i][j]))/(u[i][j]))*100;
               }



                if (acc <= change)
                {

                        check++;//condition statement to increase count if difference is greater than 0.001

                }

            }
        }
        for (j=1; j<xsize-1; j++)
        {
            for (i=1; i<ysize-1; i++)
            {
                m [i][j] = u [i][j];//filling of 2nd array
            }
        }

it++;
    }
    while(check != 0.0);
    printf("%d \t %d\n",xsize,it);
fprintf(pot,"%d \t %d \t %d\n",xsize, ysize, it);
}
   /*printf ("\n\nFinal Grid\n\n");
    do
    {

        do
        {
            printf("%.5lf \t", m[r][c]);
            fprintf(pot,"%lf \t %lf \t %lf\n",(r*h), (c*h), m[r][c] );
            c++;
        }
        while(c<xsize-1);
        printf("\n");
        r++;
        c=0;
    }
    while(r<ysize-1);

   printf("\n\nThis took %d iteartions to reach the convergence limit.", it);*/

    return 0;
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



double dub()
{
    char input[256];
    double check;
    double val;

    while (1)
    {
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &val);
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }

    return val;
}

double posdub()
{
    char input[256];
    double check;
    double d=0;

    while (1)
    {
        fgets(input, sizeof input, stdin);
        check = sscanf(input, "%lf", &d);//Check the input was the correct type
        if (check == 1) break;
        printf("Error invalid input type\n\n");
    }
    if(d<=0)//check the input was within bounds
    {
        printf("Error - percentage must be positive\n\n");
        perror(1);
    }
return d;
}


