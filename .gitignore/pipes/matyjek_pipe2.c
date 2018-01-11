#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
unsigned int n;
int i, filedesc, fd2;
double liczba, suma = 0;

sleep(1);	//sleep po to aby poprawnie uruchamiał się poprzez skrypt

filedesc = open("fifo1_2", O_RDONLY);

read(filedesc, &n, sizeof(int));


fd2 = open("fifo2_3", O_WRONLY);
write(fd2, &n, sizeof(int));

while(n)
{
suma=0;
	for(i=0;i<n;i++)
	{
	read(filedesc, &liczba, sizeof(double));
	//usleep(5000);
	suma=suma+liczba;

	printf("\nsumka= %lf\n",suma);
	}
write(fd2, &suma, sizeof(double));

read(filedesc, &n, sizeof(int));
write(fd2, &n, sizeof(int));

}

close(filedesc);
close(fd2);

return 0;
}
