#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MyTools.h"
#include <signal.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
    if( strstr(argv[0],"chart") != NULL)
    {
        signal(SIGUSR1,SignalHandler);
        signal(SIGINT,SignalHandler);
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
                
                SendViaFile(Values,measurement_ertek);
            }else if(strcmp(argv[1],"-send") == 0 && strcmp(argv[2],"-socket") == 0 || 
            strcmp(argv[2],"-send") == 0 && strcmp(argv[1],"-socket") == 0)
            {
                printf("-send\n-socket\n");
                int *Values;
                int const measurement_ertek = Measurement(&Values);
               
                SendviaSocket(Values,measurement_ertek);
            }else if(strcmp(argv[1],"-receive") == 0 && strcmp(argv[2],"-socket") == 0 || 
            strcmp(argv[2],"-receive") == 0 && strcmp(argv[1],"-socket") == 0)
            {
                printf("-receive\n-socket\n");
                ReceiveViaSocket();
            }else if(strcmp(argv[1],"-receive") == 0 && strcmp(argv[2],"-file") == 0 || 
            strcmp(argv[2],"-receive") == 0 && strcmp(argv[1],"-file") == 0)
            {
                printf("-receive\n-file\n");

                while (1)
                {
                    signal(SIGUSR1,ReceiveViaFile);
                    pause();
                }
                
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

