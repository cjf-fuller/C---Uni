#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double dub();
int posint();
double posdub();

int main()
{
    double **m=NULL, **u=NULL;
    int j,r=1,c=1,i,check,it=0,len,sep,xpos,ypos,xsize,ysize;
    int **ch=NULL;
    double change,acc,gues,pot1,pot2,ex,ey,e,ang;
    printf("\n\nPlease enter the Y length of the grid:");
    double row=posdub() +2;
    printf("\n\nPlease enter the X length of the grid:");
    double col=posdub() +2;
    printf("\n\nPlease enter the step size between nodes:");
    double h = posdub();
    xsize = (int)col/h;
    ysize = (int)row/h;

FILE *Efield;
FILE *Vpot;


    printf("SOME STUFF ABOUT ELECTRIC AND STUFF.\n\n");
    Vpot = fopen("potential-results.txt", "w");
    if (Vpot != NULL)
    {
        printf("File created containing the data for the potential\n");
    }
    else
    {
        printf("Un-able to create file\n");
    }
    Efield = fopen("efield-results.txt", "w");
    if (Efield != NULL)
    {
        printf("File created containing the data for the electric field\n");
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


            m[i][j] = 0;
            u[i][j] = 0;
            ch[i][j] = 0;

        }
    }

    do
    {
    printf("\nLength of capacitor plate:");
    len = (int)(posint())/h;
    if(len<xsize-1)
    {
        break;
    }
    printf("\nError. Please input a length that fits the grid.\n");
    }while(1);

   do
   {
    printf("\nSeparation of plates:");
    sep= (int)(posint())/h;
    if(sep<ysize-1)
    {
        break;
    }
    printf("\nError. Please input a separation that fits the grid:\n");
   }while(1);

   do
    {
    printf("\nX position of left-hand end of plate 1 (plate 2 will be position parallel below at a separation of %d):\n", sep);
    xpos = (int) (posint())/h;
   if((xpos+len)<xsize-1)//maybe not sorted.
   {
       break;
   }
   printf("\nError. Please enter a position that allows the capacitor to fit in the grid.\n");
    }while(1);

    do
        {
    printf("\nY position of left-hand end of plate 1 (plate 2 will be position parallel below at a separation of %d):\n", sep);
    ypos = (int) (posint())/h;
    if((ypos+sep)<ysize-1)//maybe not sorted
    {
        break;
    }
    printf("\nError. Please enter a position that allows the capacitor to fit in the grid.\n");
        }while(1);
    printf("Potential in plate 1:");
    pot1 = dub();
    printf("\nPotential in plate 2:");
    pot2 = dub();

printf("\n\nPlease enter the percentage change convergence condition you require:");
 acc=posdub();
 printf("\n\nPlease enter the initial guess for the grid elements to take:");
 gues = dub();

    for(i=0; i<len; i++)
    {
        m[ypos][i+xpos] = pot1;
        u[ypos][i+xpos] = pot1;
        ch[ypos][i+xpos] = 1;


        m[ypos+sep][i+xpos] = pot2;
        u[ypos+sep][i+xpos] = pot2;
        ch[ypos+sep][i+xpos] = 1;


    }


    for(j=1; j<xsize-1; j++)
    {
        for(i=1; i<ysize-1; i++)
        {
           if (m[i][j] != 0.0)
           {
               continue;
           }

            m[i][j] = gues;
            u[i][j] = gues;
        }
    }
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
         for(i=0; i<len; i++)
    {
        m[ypos][i+xpos]=pot1;
        u[ypos][i+xpos]=pot1;

        m[ypos+sep][i+xpos]=pot2;
        m[ypos+sep][i+xpos]=pot2;
    }

it++;
    }
    while(check != 0.0);

     for (i=2; i<ysize-2; i++)
    {
        for(j=2; j<xsize-2; j++)
        {
            ex = (-m[i+2][j]+8*m[i+1][j]-8*m[i-1][j]+m[i-2][j])/(12*h);//electric field components calculatins
            ey = (-m[i][j+2]+8*m[i][j+1]-8*m[i][j-1]+m[i][j-2])/(12*h);
            e = sqrt((pow(ex,2))+(pow(ey,2)));
            if(ex<0)
            {
                ang=atan(ey/ex)+M_PI;
            }
            else
            {
                ang=atan(ey/ex);
            }
            fprintf(Efield,"%lf \t %lf \t %lf \t %lf\n", (i*h), (j*h), e, ang);
        }
    }

   printf ("\n\nFinal Grid\n\n");
    do
    {

        do
        {
            printf("%lf \t", u[r][c]);
            fprintf(Vpot,"%lf \t %lf \t %lf\n", (r*h), (h*c), u[r][c]);
            c++;
        }
        while(c<xsize-1);
        printf("\n");
        r++;
        c=1;
    }
    while(r<ysize-1);
   printf("\n\nThis took %d iteartions to reach the convergence limit.", it);

    return 0;
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


