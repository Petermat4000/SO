#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<signal.h>
#include"struktury.h"

int warunek=1,warunek2 = 0 ,pid_m, pid_1, pid_3, semid, *buf_shm;

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

kill(pid_1, SIGUSR1);
kill(pid_3, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek2=0;
}

void S2(int sig)
{
opusc(semid, 0);
*buf_shm=2;
podnies(semid, 0);

kill(pid_1, SIGUSR1);
kill(pid_3, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek2=1;
}

void S1(int sig)
{
opusc(semid, 0);
*buf_shm=1;
podnies(semid, 0);

kill(pid_1, SIGUSR1);
kill(pid_3, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek=0;
warunek2=0;

}


int main()
{
struct komunikat1_2 kom;
struct komunikat2_3 kom2;
struct msqid_ds buf_WE;

int KANAL_WE, KANAL_WY, dlugosc, i=0, NR_Kanalu_1, NR_Kanalu_2, NR_semafora, NR_pamieci, shmid;
sigset_t signalSet;

int fd2_1, fd2_3, fd1_2, fd3_2;

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
printf("err semafor prog2\n");
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
NR_Kanalu_2=ftok(".",'K');


	if((KANAL_WE = msgget(NR_Kanalu_1, IPC_CREAT|0666))<0)
	{
	printf("nie moge utworzyc kanalu\n");
	exit(1);
	}

	if((KANAL_WY = msgget(NR_Kanalu_2, IPC_CREAT|0666))<0)
	{
	printf("nie moge utworzyc kanalu\n");
	exit(2);
	}

//=========przesylanie pid procesow

fd1_2=open("fifo1_2", O_RDONLY);
fd3_2=open("fifo3_2", O_RDONLY);


read(fd1_2, &pid_1, sizeof(int));
read(fd3_2, &pid_3, sizeof(int));

close(fd1_2);
close(fd3_2);


remove("fifo1_2");
remove("fifo3_2");
//===================================

while(warunek)
{
kom.linijka[0]='\0';
i=0;
dlugosc=msgrcv(KANAL_WE, &kom, ROZM_KANALU, 1, 0 );

//printf("proces 2 odebranie wiadomosci: %d\n", dlugosc);
//printf("%s\n", kom.linijka);

	while(kom.linijka[i] != '\0' && kom.linijka[i] !='\n')
	{
	i++;
	}

//printf("proces 2: %d\n", i);

kom2.typ = 2;
kom2.dlugosc = i;

dlugosc=msgsnd(KANAL_WY, &kom2, sizeof(int), 0);
//printf("proces 2 wiadomosc nadana: %d\n", dlugosc);

	while(warunek2)
	{
	pause();
	}
}
printf("prog2 koniec\n");
return 0;
}
