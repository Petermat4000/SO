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
int semid, shmid, pid, pid2, semid2,i;
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


if(semctl(semid, 0, SETVAL, 1)== -1)
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
//if(buf_shm[0]=='1')podnies(semid,0);
buf_shm[0]='1';

//========koniec tworzenia pamieci wspoldzielonej====


while(buf_shm[0]!='\0')
{
opusc(semid,1);
strncpy(tab,buf_shm,120);
podnies(semid,0);
printf("%d   %s\n",getpid(),tab);

}
return 0;
}
