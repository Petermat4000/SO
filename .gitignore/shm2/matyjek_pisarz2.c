#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include <sys/stat.h>
#include<fcntl.h>

static struct sembuf buf;

void podnies (int semid, int semnum)
{
	buf.sem_num = semnum;
	buf.sem_op = 1;
	buf.sem_flg = 0;
	if(semop(semid, &buf, 1) == -1)
	{
	printf("blad przy podnoszeniu semafora\n");
	exit(1);
	}


}

void opusc (int semid, int semnum)
{
	buf.sem_num = semnum;
	buf.sem_op = -1;
	buf.sem_flg = 0;
	if(semop(semid, &buf, 1) == -1)
	{
	printf("blad przy opuszczaniu semafora\n");
	exit(1);
	}



}

int main()
{
int NR_semafora, NR_pamieci, NR_sem2;
int semid, shmid, pid, pid2, semid2,i,lproc;
FILE *fp1;
char *buf_shm, tab[120];
struct semid_ds buf3;
struct shmid_ds buf4;

//========tworzenie semafora========

NR_semafora=ftok(".", 'G');


semid = semget(NR_semafora, 2, IPC_CREAT|0666);
if(semid==-1)
{
printf("err semafor projekt\n");
exit(2);
}


lproc=semctl(semid,1,GETNCNT,0);

if(semctl(semid, 0, SETVAL, lproc)== -1)
{
printf("blad nadania wartosci projekt\n");
exit(3);
}

if(semctl(semid, 1, SETVAL, 0)== -1)
{
printf("blad nadania wartosci projekt\n");
exit(3);
}



//========koniec tworzenia semafora

//========tworzenie pamieci wspoldzielonej====
NR_pamieci=ftok(".", 'P');
shmid = shmget(NR_pamieci, 120, IPC_CREAT|0666);
buf_shm=(char*)shmat(shmid, NULL, 0);


//========koniec tworzenia pamieci wspoldzielonej====

		if((fp1=fopen("matyjek_pisarz2.c","r"))==NULL)
		{
		printf("cos nie teges\n");
		exit(1);

		}
lproc=semctl(semid,1,GETNCNT,0);
//printf("%d\n",lproc);
	while((fgets(tab,120,fp1))!=NULL)
	{i=0;
		while(i<lproc)
		{
		opusc(semid,0);
		i++;
		}
		strncpy(buf_shm, (char*)tab, 120);
		while(i)
		{
		podnies(semid,1);	
		i--;
		}
		usleep(50000);
	
	
	}
		while(semctl(semid,0,GETVAL,0))
		{
		opusc(semid,0);
		i++;
		}
buf_shm[0]='\0';
		while(i)
		{
		podnies(semid,1);	
		i--;
		}
usleep(300000);
			semctl(semid, 0, IPC_RMID, &buf3);
			semctl(semid2, 0, IPC_RMID, &buf3);
			shmctl(shmid, IPC_RMID, &buf4);
			printf("Koncze prace\n");
return 0;
}

