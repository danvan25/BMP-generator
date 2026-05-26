#ifndef MyTools_h
#define MyTools

#define BYTE unsigned char

void version();
void help();
int Measurement(int **Values);
int szazalo(int meres_szam);
void imgGeneral(int meret,int* ertekek);
int pixelsMeret(int ertekekSzama);
int kozepVonal(int ertekekSzama);
unsigned char bitVisszaAdo(int hanyadikBit,unsigned char bajt);
int mennyiBittelLegyenEltolva(int index,int meret);
int hanyadikbaLegyenIrva(int index,int kozepvonal,int ertek,int meret);
int minertek(int meret);
int maxertek(int meret);
void BMPcreator(int *Values,int NumValues);
int FindPID();
void SendViaFile(int *Values, int NumValues);
void ReceiveViaFile(int sig);
void SendviaSocket(int *Values,int NumValues);
void ReceiveViaSocket();
void SignalHandler(int sig);
void bitTolo (int n, BYTE* bytes);

#endif