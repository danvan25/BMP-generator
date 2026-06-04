#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <omp.h>
#include "MyTools.h"

#define BYTE unsigned char
#define N 10000
#define BUFSIZE 1024
#define PORT_NO 3333
int s;


void bitTolo (int n, BYTE* bytes){
	bytes[0] = (n >> 24) & 0xFF;
	bytes[1] = (n >> 16) & 0xFF;
	bytes[2] = (n >> 8) & 0xFF;
	bytes[3] = n & 0xFF;
}


void version()
{
   #pragma omp parallel sections
    {
    #pragma omp section
      {
      printf("Elkészültének dátuma: 2024.05.03.\n");
      }
    #pragma omp section
      {
      printf("Fejlesztő neve: Magyar Dániel\n");
      }
    #pragma omp section
      {
      printf("Projekt neve: Chart.\n");
      }
    #pragma omp section
      {
      printf("Verzió: Második próbálkozás\n");
      }
    #pragma omp section
      {
      printf("Tárgy neve: Rendszerközeli programozás\n");
      }
    }
}

void help()
{
    printf("--version: kiírja a program:\n - verziószámot\n - elkészültének dátuma\n - fejlesztő neve\n");
    printf("--help: tájékoztatás a lehetséges futtatásról\n");
    printf("-send: Ez a küldő üzemmód\n");
    printf("-receive: Ez a fogadó üzemmód\n");
    printf("-file: fájl alapú kommunikációs üzemmód\n");
    printf("-socket:socket alapú kommunikációs üzemmód\n");
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



int hanyadikbaLegyenIrva(int index,int kozepvonal,int ertek,int meret)
{
    int egySorbanEnnyiBajt;

    if(pixelsMeret(meret) % meret == 0)
    {
        egySorbanEnnyiBajt = pixelsMeret(meret) / meret;
    }else
    {
        egySorbanEnnyiBajt = (pixelsMeret(meret) / meret) + 1;
    }

    int hanyadikBajtonVanASorban;

    if(index == 0 || index < 8)
    {
        hanyadikBajtonVanASorban = 0;
    }
    else 
    {
        hanyadikBajtonVanASorban = (index / 8);
    }


    int pozicio,sor;

   if(maxertek(meret) > ertek && ertek > minertek(meret))
   {
        if(ertek == 0)
        {
            sor = kozepvonal;
        }else 
        {
            if(ertek < 0)
            {
                sor = kozepvonal + ertek;
            }else
            {
                sor = kozepvonal + ertek;
            }
            
        }

   }else if( maxertek(meret) <= ertek)
   {
        sor = meret-1;
   }else if( minertek(meret) >= ertek)
   {
        sor = 0;
   }

    pozicio = sor * egySorbanEnnyiBajt + hanyadikBajtonVanASorban;
    return pozicio;
}

unsigned char bitVisszaAdo(int hanyadikBit,unsigned char bajt)
{
    unsigned char eredmeny = 0x00;

    eredmeny += 1;

    eredmeny = eredmeny << hanyadikBit;

    eredmeny = eredmeny | bajt;

    return eredmeny;
}

int mennyiBittelLegyenEltolva(int index,int meret)
{
    int eltolas = 0;

    if(index == 0)
    {
        eltolas = 7;    
    }else if(index % 8 == 0)
    {
        eltolas = 7;
    }else
    {
        eltolas = 7-(index % 8);
    }

    return eltolas;
}

int minertek(int meret)
{

    return (kozepVonal(meret))*-1;
}
int maxertek(int meret)
{
    return meret -1 - kozepVonal(meret);
}

int pixelsMeret(int ertekekSzama)
{
    int pixelM;

    if(ertekekSzama % 32 == 0)
    {
        pixelM = ertekekSzama / 32;
 
    }else
    {
        pixelM = ertekekSzama / 32 + 1;
    }


    return pixelM * 4 * ertekekSzama;
}

int kozepVonal(int ertekekSzama)
{
    return ertekekSzama / 2 + 1;
}


void BMPcreator(int *Values,int NumValues)
{
    BYTE bytes[4];
    int hossz = pixelsMeret(NumValues);
    int fullsize = 62 + hossz;

    unsigned char header[] = {
        0x42, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x3e, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x0f,
        0x00, 0x00, 0x61, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 
        0x00, 0xff

    };

    bitTolo(fullsize, bytes);

    header[2] = bytes[3];
    header[3] = bytes[2];
    header[4] = bytes[1];
    header[5] = bytes[0];

    //szelesseg
    bitTolo(NumValues, bytes);

    header[18] = bytes[3];
    header[19] = bytes[2];
    header[20] = bytes[1];
    header[21] = bytes[0];

    //magassag
    bitTolo(NumValues, bytes);

    header[22] = bytes[3];
    header[23] = bytes[2];
    header[24] = bytes[1];
    header[25] = bytes[0];

    //elso szin

    header[54] = 0xff;
    header[55] = 0xff;
    header[56] = 0xff;

    //masodik szin

    header[58] = 0x00;
    header[59] = 0x00;
    header[60] = 0x00;

    //----------------------
    
    
    unsigned char pixels[hossz];

    for(int i=0;i<hossz;i++)
    {
        pixels[i] = 0x00;
    }

    
    int out = open("chart.bmp",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRGRP|S_IROTH);
    
    if(out == -1)
    {
        fprintf(stderr,"A kep letrehozasa/megnyitasa kozben hiba lepett fel.");
    }else
    {
        write(out,header,62);
        write(out,pixels,hossz);
        int kozepvonal = kozepVonal(NumValues);
        
        for(int i=0;i<NumValues;i++)
        {
            int sajt = hanyadikbaLegyenIrva(i,kozepVonal(NumValues),Values[i],NumValues);
            int kurzorPoz =  62 + sajt;
            lseek(out,kurzorPoz,SEEK_SET);
            unsigned char utolsoBajt; 
            read(out,&utolsoBajt,1);
            lseek(out,-1,SEEK_CUR);
            unsigned char bajtErtek = bitVisszaAdo(mennyiBittelLegyenEltolva(i,NumValues),utolsoBajt);
            write(out,&bajtErtek,1);
        }
           
    }

    close(out);

}

int FindPID() {
    DIR *dir;
    struct dirent *entry;
    FILE *fp;
    char filepath[N];
    char buffer[1024];
    char name[256];
    int pid = -1;
    int my_pid = getpid();

    dir = opendir("/proc"); 

    if (dir == NULL) 
    {
        printf("Error opening directory\n");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        
        if (isdigit(entry->d_name[0])) {
            sprintf(filepath, "/proc/%s/status", entry->d_name);
            //printf("%s\n",filepath);
            fp = fopen(filepath, "r");

            if (fp != NULL) {

                fscanf(fp, "Name:\t%s", name);
                // keressuk a --> "bash" vagy a "chart" attol fugg melyik van megadva eppen
                if (strcmp(name, "chart") == 0) {
                    // pid kereses
                    while (fgets(buffer, sizeof(buffer), fp)) {
                        if (fscanf(fp, "Pid:\t%d", &pid) == 1) //&pid segitsegevel egybol a pid be kerul, elkeruljuk a folosleges konvertalast
                        {
                            if(pid == my_pid)
                            {
                                pid = -1; //ha csak egyetlen egy chart lenne elinditva
                                continue;
                            }
                        }
                    }

                    fclose(fp);
                    closedir(dir);
                    return pid;
                }else
                {
                    pid = -1;
                }

                fclose(fp);
            }
        }
    }

    closedir(dir);

    return pid;
}

void SendViaFile(int *Values, int NumValues)
{
    char  *value;
    char filepath[N];
    value = getenv("HOME");
    
    sprintf(filepath, "%s/Measurement.txt",value);

    FILE* fp = fopen(filepath,"w");

    if(fp == NULL)
    {
        fprintf(stderr,"Hiba a Measurement.txt fajl kiiratas kozben!");
    }else
    {
        for(int i=0;i<NumValues;i++)
        {
            fprintf(fp,"%d\n",Values[i]);
        }
    }

    fclose(fp);
    int FPid = FindPID();
    if(-1 == FPid)
    {
        fprintf(stderr,"Nincs talalat fogado uzemmodban mukodo folyamat (process)!\n");
        exit(-1);
    }else
    {
        //Ide kell a signal
        kill(FPid,SIGUSR1);
    }
}

void ReceiveViaFile(int sig)
{
    char  *value;
    char filepath[N];
    value = getenv("HOME");
    
    sprintf(filepath, "%s/Measurement.txt",value);

    FILE* fp = fopen("/home/danvan25/Measurement.txt","r");
    int num_of_chars =1;
    char** char_array_ptr;


    if(fp == NULL)
    {
        fprintf(stderr,"Hiba a Measurement.txt fajl beolvasas kozben!");
    }else
    {
        int i= 0;
        char sor[1000];
        int szam;


        while(fgets(sor,1000,fp))
        {   
            i++;
        }
        fclose(fp);

        FILE* fp = fopen(filepath,"r");
        
        num_of_chars= i;
        int* int_array= (int*)malloc(num_of_chars*sizeof(int));
        //char_array_ptr = malloc(num_of_chars * sizeof(char *));
        i=0;
        
        while(fgets(sor,1000,fp))
        {  
            int_array[i]= atoi(sor);
            //sor[strlen(sor)-1] ='\0';
            //char_array_ptr[i] = malloc(strlen(sor) * sizeof(char));
            //strcpy(char_array_ptr[i], sor);
            //printf("%s\n",char_array_ptr[i]);
            i++;
        }

        fclose(fp);

        /*for(int i=0;i<num_of_chars;i++)
        {
            printf("%d %d\n",i,int_array[i]);
        }*/

        BMPcreator(int_array,num_of_chars);
        free(int_array);        

    }

}



void SendviaSocket(int *Values,int NumValues)
{
    int s;                            
   int send_bytes;  
   int rec_bytes;                      
   int flag;                         
   char on;                          
   char buffer[BUFSIZE];             
   unsigned int server_size;         
   struct sockaddr_in server;
   int bejovo_num;       

   on   = 1;
   flag = 0;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;

   s = socket(AF_INET, SOCK_DGRAM, 0 );
   if ( s < 0 ) {
      fprintf(stderr, "Socket creation error.\n");
      exit(2);
      }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);
    
   send_bytes = sendto(s, &NumValues, sizeof(int), flag, (struct sockaddr *) &server, server_size);
   if ( send_bytes <= 0 ) 
    {
        fprintf(stderr, "Sending error.\n");
        exit(3);
    }
    printf (" %i bytes have been sent to server.\n", send_bytes-1);
    printf("Elkuldott: %d\n",NumValues);
    signal(SIGALRM,SignalHandler);
    alarm(1);
    rec_bytes = recvfrom(s, &bejovo_num, sizeof(int32_t), flag, (struct sockaddr *) &server, &server_size);

    if ( bejovo_num != NumValues ) 
    {
      fprintf(stderr, "Receiving error.\n");
      printf (" %i bytes have been sent to server.\n", rec_bytes-1);
      printf("bejovo: %d\n",bejovo_num); 
      exit(4);
    }

    send_bytes = sendto(s, Values,NumValues*sizeof(int32_t), flag, (struct sockaddr *) &server, server_size);
    
    rec_bytes = recvfrom(s, &bejovo_num, sizeof(int32_t), flag, (struct sockaddr *) &server, &server_size);

    if(bejovo_num != send_bytes)
      {
        fprintf(stderr, "Receiving error.\n");
        printf (" %i bytes have been sent to server.\n", rec_bytes-1);
        printf("bejovo: %d\n",bejovo_num); 
        exit(4);
      }
   close(s);
}
void ReceiveViaSocket()
{
    int bytes;                        // received/sent bytes
    int err;                          // error code
    int flag;                         // transmission flag
    char on;                          // sockopt option
    unsigned int server_size;         // length of the sockaddr_in server
    unsigned int client_size;         // length of the sockaddr_in client
    struct sockaddr_in server;        // address of server
    struct sockaddr_in client;        // address of client
    int NumValues;


   on   = 1;
   flag = 0;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;
   client_size = sizeof client;

   s = socket(AF_INET, SOCK_DGRAM, 0 );
   if ( s < 0 ) {
      fprintf(stderr, "Socket creation error.\n");
      exit(2);
      }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   err = bind(s, (struct sockaddr *) &server, server_size);
   if ( err < 0 ) {
      fprintf(stderr,"Binding error.\n");
      exit(3);
      }

   while(1)
   { 

      printf("\n Waiting for a message...\n");
      bytes = recvfrom(s, &NumValues, sizeof(int32_t), flag, (struct sockaddr *) &client, &client_size );

      if ( bytes < 0 ) 
      {
         fprintf(stderr, "Receiving error.\n");
         exit(4);
      }
      //printf("%d\n",NumValues);
      bytes = sendto(s, &NumValues, sizeof(int32_t), flag, (struct sockaddr *) &client, client_size);

      
      if ( bytes <= 0 ) 
      {
         fprintf(stderr, "Sending error.\n");
         exit(5);
      }
         
      int* a=(int*)malloc(NumValues*sizeof(int32_t));
      bytes = recvfrom(s,a, NumValues*sizeof(int32_t), flag, (struct sockaddr *) &client, &client_size );

      bytes = sendto(s, &bytes, sizeof(int32_t), flag, (struct sockaddr *) &client, client_size);

      BMPcreator(a,NumValues);

      free(a);

   }

   close(s);
}
void SignalHandler(int sig)
{
    
    if(sig == SIGINT)
    {
        printf("\nA program leáll...\n");
        exit(0);
    }else if(sig == SIGUSR1)
        {
            fprintf(stderr,"Fájlon keresztüli szolgáltatás nem elérhető!\n");
        }else if(sig == SIGALRM)
        {
            fprintf(stderr,"A szerver nem válaszol (időkereten belül).\n");
            exit(-1);
        }

}