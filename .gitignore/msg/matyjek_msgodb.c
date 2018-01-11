#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
struct komunikat
{
long typ;
char linijka[100];
int pid;
};


int main()
{
key_t NR_kolejki;
int Kanal_WE, dlugosc, moje_pid=getpid();
//struct msqid_ds buf_WE;
struct komunikat kom;

NR_kolejki = ftok(".", 'S');

Kanal_WE=msgget(NR_kolejki, IPC_CREAT|0666);

printf("numer kolejki: %d\n",NR_kolejki);
printf("moje pid: %d\n",getpid());
do
{
dlugosc=msgrcv(Kanal_WE, &kom, 100+sizeof(int), moje_pid, 0);
printf("%d\t",moje_pid);

printf("%s\n",kom.linijka);


}while(kom.linijka[0]!='@' && dlugosc>0);

//msgctl(Kanal_WE, IPC_RMID, &buf_WE);
return 0;
}
