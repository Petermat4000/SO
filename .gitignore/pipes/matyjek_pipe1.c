#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()
{
int i, filedesc;
unsigned int n;
double *tab;

mkfifo("fifo1_2", S_IWUSR | S_IRUSR );
mkfifo("fifo2_3", S_IWUSR | S_IRUSR );

filedesc = open("fifo1_2", O_WRONLY);

printf("podaj ile liczb:  ");
scanf("%d", &n);

write(filedesc, &n, sizeof(int));

while(n)
{

tab = malloc(sizeof(double)*n);

	for(i=0;i<n;i++)
	{
	printf("podaj liczbe rzeczywista: ");
	scanf("%lf", &tab[i]);
	
	write(filedesc, &tab[i], sizeof(double));
	
	}

printf("podaj ile liczb:  ");
scanf("%d", &n);

write(filedesc, &n, sizeof(int));

free(tab);

}


close(filedesc);

remove("fifo1_2");
remove("fifo2_3");

return 0;
}

