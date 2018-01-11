#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<signal.h>
#include"struktury.h"

int warunek=1, warunek2=0,  pid_m, pid_2, pid_3, semid, *buf_shm;

void S4(int sig)
{
int akcja=0;
	opusc(semid, 0);
	akcja=(*buf_shm);
	podnies(semid, 0);
	
	switch (akcja)
	{
	case 1:
	warunek = 0;
	warunek2 = 0;
	raise(SIGKILL);
	break;
	case 2:
	warunek2 = 1;
	break;
	case 3:
	warunek2 = 0;
	break;
	}
}

void S3(int sig)
{
opusc(semid, 0);
*buf_shm=3;
podnies(semid, 0);

kill(pid_2, SIGUSR1);
kill(pid_3, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek2=0;

}

void S2(int sig)
{
opusc(semid, 0);
*buf_shm=2;
podnies(semid, 0);

kill(pid_2, SIGUSR1);
kill(pid_3, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek2=1;
}

void S1(int sig)
{
opusc(semid, 0);
*buf_shm=1;
podnies(semid, 0);

kill(pid_2, SIGUSR1);
kill(pid_3, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek=0;
warunek2=0;
raise(SIGKILL);

}

int main()
{
struct komunikat1_2 kom;
struct msqid_ds buf_WY;
int KANAL_WY,  warunek=1, size=256, c, NR_Kanalu_1, NR_semafora, NR_pamieci, shmid, i;
char tekst[256];

sigset_t signalSet;
int fd1_2, fd1_3, fd2_1, fd3_1;

int  pid_moj=getpid();
pid_m=getppid();

//=======ustawienie maski sygnalow================

	if(sigemptyset(&signalSet))printf("error!\n");
	if(sigfillset(&signalSet))printf("errrrrrr!\n");
	if(sigdelset(&signalSet,SIGUSR1))printf("error!\n");
	if(sigdelset(&signalSet,SIGUSR2))printf("error!\n");
	if(sigdelset(&signalSet,SIGALRM))printf("error!\n");
	if(sigdelset(&signalSet,SIGILL))printf("error!\n");
	sigprocmask(SIG_SETMASK, &signalSet,NULL);

//======koniec ustawiania maski sygnalow======

//======uzyskanie semafora===============

NR_semafora=ftok(".", 'G');

semid = semget(NR_semafora, 1, IPC_CREAT|0666);
if(semid==-1)
{
printf("err semafor prog1\n");
exit(2);
}

//====koniec uzyskiwania semafora======

//========uzyskiwanie pamieci wspoldzielonej====
NR_pamieci=ftok(".", 'P');
shmid = shmget(NR_pamieci, sizeof(int), 0666);
buf_shm=(int*)shmat(shmid, NULL, 0);


//========koniec uzyskiwania pamieci wspoldzielonej====

//=====obsluga sygnalu========

signal(SIGUSR1, S4);
signal(SIGUSR2, S3);
signal(SIGALRM, S2);
signal(SIGILL, S1);

//======koniec obslugi sygnalu==========

NR_Kanalu_1=ftok(".",'M');

	if((KANAL_WY = msgget(NR_Kanalu_1, IPC_CREAT|0666))<0)
	{
	printf("nie moge utworzyc kanalu\n");
	exit(1);
	}

//=========przesylanie pid procesow

fd2_1=open("fifo2_1", O_RDONLY);
fd3_1=open("fifo3_1", O_RDONLY);

read(fd2_1, &pid_2, sizeof(int));
read(fd3_1, &pid_3, sizeof(int));

close(fd2_1);
close(fd3_1);

remove("fifo2_1");
remove("fifo3_1");
//===================================

while(warunek)
{

	usleep(500);
	printf("wpisz tekst\n");

	fgets(tekst, size, stdin);
	while(warunek2)
	{
	pause();
	}

	//printf("%s\n", tekst);
	strncpy(kom.linijka, (char *)tekst, 256);
	kom.typ=1;
	//printf("%s\n",kom.linijka);
	c = msgsnd(KANAL_WY, &kom, 256, 0);
	//printf("czy komunikat wyslany: %d\n",c);

}
printf("prog1 koniec\n");
return 0;
}
