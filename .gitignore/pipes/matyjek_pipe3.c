#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
unsigned int n;
int i, filedesc;
double liczba, suma = 0;

sleep(1);	//sleep po to aby poprawnie uruchamiał się poprzez skrypt

filedesc = open("fifo2_3", O_RDONLY);

read(filedesc, &n, sizeof(int));


while(n)
{
read(filedesc, &suma, sizeof(double));

liczba=suma/n;

printf("srednia = %lf\n",liczba);

read(filedesc, &n, sizeof(int));

}

close(filedesc);

return 0;
}
