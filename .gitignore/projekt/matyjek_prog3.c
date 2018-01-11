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

int warunek=1,warunek2=0, pid_m, pid_1, pid_2, semid, *buf_shm;

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

kill(pid_2, SIGUSR1);
kill(pid_1, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek2=0;
}

void S2(int sig)
{
opusc(semid, 0);
*buf_shm=2;
podnies(semid, 0);

kill(pid_2, SIGUSR1);
kill(pid_1, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek2=1;
}

void S1(int sig)
{
opusc(semid, 0);
*buf_shm=1;
podnies(semid, 0);

kill(pid_2, SIGUSR1);
kill(pid_1, SIGUSR1);
kill(pid_m, SIGUSR1);
warunek=0;
warunek2=0;

}

int main()
{
struct komunikat2_3 kom;
sigset_t signalSet;

int KANAL_WE,  dlugosc,  NR_Kanalu_2, NR_semafora, NR_pamieci, shmid;


int fd3_1, fd3_2, fd2_3, fd1_3;

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
printf("err semafor prog3\n");
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

NR_Kanalu_2=ftok(".",'K');

if((KANAL_WE = msgget(NR_Kanalu_2, IPC_CREAT|0666))<0)
	{
	printf("nie moge utworzyc kanalu\n");
	exit(1);
	}

//=========przesylanie pid procesow


fd1_3=open("fifo1_3", O_RDONLY);
fd2_3=open("fifo2_3", O_RDONLY);

read(fd1_3, &pid_1, sizeof(int));
read(fd2_3, &pid_2, sizeof(int));

close(fd1_3);
close(fd2_3);

remove("fifo1_3");
remove("fifo2_3");
//===================================

while(warunek)
{
kom.dlugosc=0;
dlugosc=msgrcv(KANAL_WE, &kom, sizeof(int), 2, 0);
//printf("wiadomosc odebrana: %d\n", dlugosc);

	while(warunek2)
	{
	pause();
	}

printf("proces 3: %d\n", kom.dlugosc);

}


printf("prog3 koniec\n");

return 0;
}
