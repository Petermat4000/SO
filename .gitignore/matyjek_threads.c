#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct ele
{
double x;
int i;
}elementy;




void *wyraz (void *arg)
{
elementy *point=(elementy*)arg;

int i, f,g;
double iks_kw, minus_jeden, *wynik, x, silnia=1;

i=point->i;
x=point->x;

wynik=(double*)malloc(sizeof(double));

f=2*i;

//printf("%f  ",point->x);
minus_jeden=pow((double)(-1),(double)i);
iks_kw=pow(x,(double)(2*i));				//zmiana1

for(g=1;g<=f;g++)
{
silnia=silnia*g;
}

*wynik=(minus_jeden*iks_kw)/silnia;
//printf("%f   ", *wynik);				zmiana2

pthread_exit(wynik);



}

int main(int argc, char *argv[])
{
int N,  i;
double *wynik, suma=0, x;
elementy *T;
pthread_t *L;

sscanf(argv[1],"%lf",&x);			//konwersja pierwszego argumentu na double
sscanf(argv[2],"%d",&N);			//konwersja drugiego argumentu na int

T=malloc(sizeof(elementy)*(N+1));
L=malloc(sizeof(pthread_t)*(N+1));

for(i=0;i<=N;i++)
{
T[i].x=x;
T[i].i=i;
//printf("%f   ", x);
pthread_create(&L[i],NULL, wyraz,&T[i]);

}

sleep(2);

for(i=0;i<=N;i++)
{
pthread_join(L[i], (void**)&wynik);

printf("%ld\n", (long)L[i]);

suma=suma+(*wynik);

free(wynik);				//zwalnianie pamieci dla wynik

}

free(T);				//zwalnianie pamięci dla tablicy struktur
free(L);				//zwalnianie pamięci dla tablicy wątków
printf("wynik: %f\n",suma);

printf("%f\n", cos(x));			//wypisanie wartosci cos(x) dla sprawdzenia poprawności

return 0;
}

