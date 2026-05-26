#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//void version();
//void help();
//int Measurement(int **Values);
//int szazalo(int meres_szam);


int main(int argc,char* argv[])
{
    if( strstr(argv[0],"chart") != NULL)
    {

        if(argc == 1)
        {
            printf("alapertelmezett beallitas:\n");
            printf("--send\n");
            printf("--file\n");
            int *Values;
            int const measurement_ertek = Measurement(&Values);
            for(int i=0;i<measurement_ertek;i++)
            {
                printf("%d\n",Values[i]);
            }
            return 0;
        }else if(argc == 2)
        {
            if( strcmp(argv[1],"--version") == 0 )
            {
                version();
            }else if( strcmp(argv[1],"--help") == 0 )
            {
                help();
            }else if( strcmp(argv[1],"-send") == 0 )
            {   
                printf("-send\n");
                printf("-file\n");
                int *Values;
                int const measurement_ertek = Measurement(&Values);
                for(int i=0;i<measurement_ertek;i++)
                {
                    printf("%d\n",Values[i]);
                }
            }else if( strcmp(argv[1],"-receive") == 0 )
            {
                printf("-receive\n");
                printf("-file\n");
                
            }else if( strcmp(argv[1],"-file") == 0 )
            {
                printf("-file\n");
                printf("-send\n");
                int *Values;
                int const measurement_ertek = Measurement(&Values);
                for(int i=0;i<measurement_ertek;i++)
                {
                    printf("%d\n",Values[i]);
                }
            }else if( strcmp(argv[1],"-socket") == 0 )
            {
                printf("-socket\n"); 
                printf("-send\n");
                int *Values;
                int const measurement_ertek = Measurement(&Values);
                for(int i=0;i<measurement_ertek;i++)
                {
                    printf("%d\n",Values[i]);
                }
            }else 
            {
                printf("Hibas argumentum megadas!\n");
                help();
                return 0;
            }
        }else if(argc == 3)
        {
            if( strcmp(argv[1],"-send") == 0 && strcmp(argv[2],"-file") == 0 || 
            strcmp(argv[2],"-send") == 0 && strcmp(argv[1],"-file") == 0)
            {
                printf("-send\n-file\n");
                int *Values;
                int const measurement_ertek = Measurement(&Values);
                
               SendViaFile(&Values,measurement_ertek);
            }else if(strcmp(argv[1],"-send") == 0 && strcmp(argv[2],"-socket") == 0 || 
            strcmp(argv[2],"-send") == 0 && strcmp(argv[1],"-socket") == 0)
            {
                printf("-send\n-socket\n");
                int *Values;
                int const measurement_ertek = Measurement(&Values);
               
                SendviaSocket(&Values,measurement_ertek);
            }else if(strcmp(argv[1],"-receive") == 0 && strcmp(argv[2],"-socket") == 0 || 
            strcmp(argv[2],"-receive") == 0 && strcmp(argv[1],"-socket") == 0)
            {
                printf("-receive\n-socket\n");
                ReceiveViaSocket();
            }else if(strcmp(argv[1],"-receive") == 0 && strcmp(argv[2],"-file") == 0 || 
            strcmp(argv[2],"-receive") == 0 && strcmp(argv[1],"-file") == 0)
            {
                printf("-receive\n-file\n");
                ReceiveViaFile();
            }else
            {
                printf("Hibas argumentum megadas!\n");
                help();
                return 0;
            }
        }else if(argc > 3)
        {
            printf("Hibas argumentum megadas!\n");
            help();
            return 0;
        }

    }else
    {
        fprintf(stderr,"Nem chart a neve a futtatható állománynak!\n");
        return -1;
    }


    return EXIT_SUCCESS;
}

/*void version()
{
    printf("Verziószám: 2.0\n");
    printf("Elkészültének dátuma: 2024.02.24\n");
    printf("Fejlesztő neve: Magyar Dániel\n");
}

void help()
{
    printf("--version : blabla\n");
    printf("--help : blabla\n");
    printf("-send : blabla\n");
    printf("-receive : blabla\n");
    printf("-file : blabla\n");
    printf("-socket : blabla\n");
}

int Measurement(int **Values)
{
    int meres_szamok = 0;
    
    time_t seconds;
    struct tm *T4;

    seconds = time(NULL);

    T4 = localtime(&seconds);
    //printf("%d:%d\n",(*T4).tm_min,(*T4).tm_sec);

    //printf("tm_sec: %d\n",(*T4).tm_sec);
    //printf("tm_min: %d\n",(*T4).tm_min);

        if( (*T4).tm_min >= 0 && 15 > (*T4).tm_min )
        {
            meres_szamok = (*T4).tm_min * 60 + (*T4).tm_sec;
            meres_szamok = szazalo(meres_szamok);
        }else if((*T4).tm_min >= 15 && 30 > (*T4).tm_min)
        {
            meres_szamok = ((*T4).tm_min - 15) * 60 + (*T4).tm_sec;
            meres_szamok = szazalo(meres_szamok);
        }else if((*T4).tm_min >= 30 && 45 > (*T4).tm_min)
        {
            meres_szamok = ((*T4).tm_min - 30) * 60 + (*T4).tm_sec;
        }else if((*T4).tm_min >= 45)
        {
           meres_szamok = ((*T4).tm_min - 45) * 60 + (*T4).tm_sec;
           meres_szamok = szazalo(meres_szamok);
        }

    *Values = (int*)malloc(meres_szamok*sizeof(int));    

    //int szamlalo[3] = {0,0,0};

    double rf;

    srand(time(NULL));

    (*Values)[0]=0;
    for(int i=1;i<meres_szamok;i++)
    {
        rf = (double)rand()/((unsigned)RAND_MAX+1); 
        if(rf<0.428571) 
        {
            //szamlalo[0]++;
            (*Values)[i]=(*Values)[i-1]+1;
        }  
        else if(rf < 0.78341)
        {
            //szamlalo[1]++;
            (*Values)[i]=(*Values)[i-1]-1;
        }else
            {
                //szamlalo[(int)((rf-0.2)/((1.0-0.2)/2))+1]++;
                (*Values)[i]=(*Values)[i-1];
            }      
        //int d = (int)((rf-0.2)/((1.0-0.2)/2))+1;
        //printf("%d\n",d);
    }

    return meres_szamok;

}

int szazalo(int meres_szam)
{
    if(meres_szam < 100)
    {
        return 100;
    }
    else
    {
        return meres_szam;
    }
}
*/