#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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

int warunek=1, pid, pid2, pid3, semid, *buf_shm;
void S4(int sig)
{
	int akcja=0;
	opusc(semid, 0);
	akcja=(*buf_shm);
	podnies(semid, 0);
	if(akcja==1)
	warunek=0;
}

void S3(int sig)
{
opusc(semid, 0);
*buf_shm=3;
podnies(semid, 0);

kill(pid, SIGUSR1);
kill(pid2, SIGUSR1);
kill(pid3, SIGUSR1);

}

void S2(int sig)
{
opusc(semid, 0);
*buf_shm=2;
podnies(semid, 0);

kill(pid, SIGUSR1);
kill(pid2, SIGUSR1);
kill(pid3, SIGUSR1);
}

void S1(int sig)
{
	opusc(semid, 0);
	*buf_shm=1;
	podnies(semid, 0);

	kill(pid, SIGUSR1);
	kill(pid2, SIGUSR1);
	kill(pid3, SIGUSR1);
	
	warunek = 0;

}

int main()
{
int NR_Kanalu_1, NR_Kanalu_2,  NR_semafora, NR_pamieci, shmid;
int KANAL1, KANAL2;
int fd1_2, fd1_3, fd2_1, fd2_3, fd3_1, fd3_2;

struct msqid_ds buf1, buf2;
struct semid_ds buf3;
struct shmid_ds buf4;

sigset_t signalSet;

//=======ustawienie maski sygnalow================

	if(sigemptyset(&signalSet))printf("error!\n");
	if(sigfillset(&signalSet))printf("errrrrrr!\n");
	if(sigdelset(&signalSet,SIGUSR1))printf("error!\n");
	if(sigdelset(&signalSet,SIGUSR2))printf("error!\n");
	if(sigdelset(&signalSet,SIGALRM))printf("error!\n");
	if(sigdelset(&signalSet,SIGILL))printf("error!\n");
	sigprocmask(SIG_SETMASK, &signalSet,NULL);

//======koniec ustawiania maski sygnalow======

//========tworzenie semafora========

NR_semafora=ftok(".", 'G');

semid = semget(NR_semafora, 1, IPC_CREAT|0666);
if(semid==-1)
{
printf("err semafor projekt\n");
exit(2);
}

if(semctl(semid, 0, SETVAL, 1)== -1)
{
printf("blad nadania wartosci projekt\n");
exit(3);
}

//========koniec tworzenia semafora

//========tworzenie pamieci wspoldzielonej====
NR_pamieci=ftok(".", 'P');
shmid = shmget(NR_pamieci, sizeof(int), IPC_CREAT|0666);
buf_shm=(int*)shmat(shmid, NULL, 0);


//========koniec tworzenia pamieci wspoldzielonej====

//=====obsluga sygnalu========

signal(SIGUSR1, S4);
signal(SIGUSR2, S3);
signal(SIGALRM, S2);
signal(SIGILL, S1);

//======koniec obslugi sygnalu==========

//========tworzenie kanalow lacznosci================

NR_Kanalu_1=ftok(".",'M');
NR_Kanalu_2=ftok(".",'K');

	if((KANAL1=msgget(NR_Kanalu_1, IPC_CREAT|0666))<0)
	{
	printf("blad utworzenia kolejki 1\n");
	exit(1);

	}

	if((KANAL2=msgget(NR_Kanalu_2, IPC_CREAT|0666))<0)
	{
	printf("blad utworzenia kolejki 2\n");
	exit(2);

	}

//======koniec tworzenia kanalow lacznosci===========

//======Tworzenie laczy=========
mkfifo("fifo1_2", S_IWUSR | S_IRUSR);
mkfifo("fifo1_3", S_IWUSR | S_IRUSR);

mkfifo("fifo2_1", S_IWUSR | S_IRUSR);
mkfifo("fifo2_3", S_IWUSR | S_IRUSR);

mkfifo("fifo3_1", S_IWUSR | S_IRUSR);
mkfifo("fifo3_2", S_IWUSR | S_IRUSR);

//====koniec tworzenia laczy

if((pid=fork())<0)
{
printf("blad tworzenia potomka\n");
exit(1);
}
else
{
	if(pid==0)
	{
	
	execl("matyjek_prog1.o",NULL);

	}
	else
	{
	
	if((pid2=fork())<0)
		{
		printf("blad utworzenia potomka\n");
		exit(2);
		}	
		else
		{
			if(pid2==0)
			{		
			
			execl("matyjek_prog2.o",NULL);

			}
			else
			{

			if((pid3=fork())<0)
				{
				printf("blad utworzenia potomka\n");
				exit(2);
				}	
				else
				{
					if(pid3==0)
					{		
			
					execl("matyjek_prog3.o",NULL);

					}
					else
					{}		
		
		
				}

			}		
		
		
		}
	
	}


}
printf("pid macierzysty: %d\n", getpid());
printf("program1: %d    \nprogram2: %d     \nprogram3: %d\n", pid, pid2, pid3);

//=======przeslanie pid-ow do procesow potomnych======================

fd2_1=open("fifo2_1", O_WRONLY);
fd3_1=open("fifo3_1", O_WRONLY);

fd1_2=open("fifo1_2", O_WRONLY);
fd3_2=open("fifo3_2", O_WRONLY);

fd1_3=open("fifo1_3", O_WRONLY);
fd2_3=open("fifo2_3", O_WRONLY);

write(fd2_1, &pid2, sizeof(int));
write(fd3_1, &pid3, sizeof(int));

write(fd1_2, &pid, sizeof(int));
write(fd3_2, &pid3, sizeof(int));

write(fd1_3, &pid, sizeof(int));
write(fd2_3, &pid2, sizeof(int));

usleep(300000);

close(fd2_1);
close(fd3_1);
close(fd1_2);
close(fd3_2);
close(fd1_3);
close(fd2_3);

//=============koniec przesylania pid-ow===================


while(warunek)
{
pause();
}
usleep(300000);
msgctl(KANAL1, IPC_RMID, &buf1);
msgctl(KANAL2, IPC_RMID, &buf2);
semctl(semid, 0, IPC_RMID, &buf3);
shmctl(shmid, IPC_RMID, &buf4);
printf("prog-projekt koniec\n");
return 0;
}
