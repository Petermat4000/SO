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

int main(int argc, char *argv[])
{
key_t NR_kolejki;
int Kanal_WY, dlugosc, pid_adr, cont;
struct msqid_ds buf_WY;
struct komunikat kom;
char tekst[100], c;

sscanf(argv[1], "%d", &NR_kolejki);
Kanal_WY=msgget(NR_kolejki, IPC_CREAT|0666);
do
{
	printf("podaj teskt\n");
	//printf("%d\n", NR_kolejki);
	fgets(tekst, 100, stdin);
	printf("podaj pid odbiorcy\n");
	//printf("%s\n", tekst);
	scanf("%d",&pid_adr);
		//bardzo wazna czesc kodu. nigdy przenigdy nie wyrzucaj tego!!!!!!
	while((c = getchar())!= '\n' && c != EOF)
	{
	printf("bla\n");
	}



	kom.typ = pid_adr;
	kom.pid=pid_adr;
	strncpy(kom.linijka, tekst, 100);


	cont=msgsnd(Kanal_WY, &kom, 100+sizeof(int),0);
	
	if(cont<0)
	{
	printf("blad wyslania komunikatu\n");
	return 2;
	}
}while(tekst[0]!='@');
msgctl(Kanal_WY, IPC_RMID, &buf_WY);
return 0;
}
