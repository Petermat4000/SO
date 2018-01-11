#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

double wynik;

pthread_mutex_t mojmutex=PTHREAD_MUTEX_INITIALIZER;		//zapis
pthread_mutex_t mojmutex2=PTHREAD_MUTEX_INITIALIZER;		//odczyt

typedef struct ele
{
double x;
int i;
}elementy;

void *wyraz (void *arg)
{

//pthread_mutex_lock(&mojmutex2);
pthread_mutex_lock(&mojmutex);

elementy *point=(elementy*)arg;

int i, f,g;
double iks_kw, minus_jeden, x, silnia=1;


i=point->i;
x=point->x;

f=i;

iks_kw=pow(x,(double)(i));			

for(g=1;g<=f;g++)
{
silnia=silnia*g;
}

wynik=(iks_kw)/silnia;
pthread_mutex_unlock(&mojmutex2);
			
return 0;

}

int main(int argc, char *argv[])
{
int N=10,  i;
double suma=0, x;
elementy T[10];
pthread_t L[10];

sscanf(argv[1],"%lf",&x);			//konwersja pierwszego argumentu na double


pthread_mutex_lock(&mojmutex);

for(i=0;i<N;i++)
{
T[i].x=x;
T[i].i=i;

pthread_create(&L[i],NULL, wyraz,&T[i]);
pthread_detach(L[i]);

}

pthread_mutex_unlock(&mojmutex);
usleep(10000);

for(i=0;i<N;i++)
{


printf("%ld       ", (long)L[i]);

pthread_mutex_lock(&mojmutex2);
printf("%lf\n", wynik);
suma=suma+(wynik);
pthread_mutex_unlock(&mojmutex);

}

printf("suma: %f\n",suma);

printf("exp(x)=   %f\n", exp(x));	//wypisanie wartosci funkcji e(x) dla sprawdzenia poprawności

return 0;
}

