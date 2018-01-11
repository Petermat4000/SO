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
int semid, shmid, pid, pid2, semid2;
FILE *fp1, *fp2;
char *buf_shm;
struct semid_ds buf3;
struct shmid_ds buf4;

//========tworzenie semafora========

NR_semafora=ftok(".", 'G');
NR_sem2=ftok(".", 'A');

semid = semget(NR_semafora, 3, IPC_CREAT|0666);
if(semid==-1)
{
printf("err semafor projekt\n");
exit(2);
}

semid2 = semget(NR_sem2, 1, IPC_CREAT|0666);
if(semid2==-1)
{
printf("err semafor projekt22\n");
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

if(semctl(semid, 2, SETVAL, 0)== -1)
{
printf("blad nadania wartosci projekt\n");
exit(3);
}

if(semctl(semid2, 0, SETVAL, 1)== -1)
{
printf("blad nadania wartosci projekt222\n");
exit(3);
}

//========koniec tworzenia semafora

//========tworzenie pamieci wspoldzielonej====
NR_pamieci=ftok(".", 'P');
shmid = shmget(NR_pamieci, 160, IPC_CREAT|0666);
buf_shm=(char*)shmat(shmid, NULL, 0);


//========koniec tworzenia pamieci wspoldzielonej====

if((pid=fork())<0)
{
printf("blad tworzenia potomka\n");
exit(1);
}
else
{
	if(pid==0)
	{
		char tab[160];
		char tab2[4];
		int pid=getpid(),i;		
		
		sprintf(tab2, "%d", pid);
		for(i=0;i<4;i++)
		tab[i]=tab2[i];
		tab[4]=' ';
		
		if((fp1=fopen("/etc/profile","r"))==NULL)
		{
		printf("cos nie teges\n");
		exit(1);

		}
		
			while((fgets(&tab[5], 155, fp1))!=NULL)
			{
			
			opusc(semid, 0);
			
			strncpy(buf_shm, (char*)tab, 160);
		
			podnies(semid,2);
			usleep(50000);
			
			}
		
		opusc(semid2,0);
		fclose(fp1);
		

	}
	else
	{
		if((pid2=fork())==0)
		{
			char tab[160];
			char tab2[4];
		int pid=getpid(),i;		
		
		sprintf(tab2, "%d", pid);
		for(i=0;i<4;i++)
		tab[i]=tab2[i];
		tab[4]=' ';

		if((fp2=fopen("matyjek_shm.c","r"))==NULL)
		{
		printf("cos nie teges\n");
		exit(1);

		}
			while((fgets(&tab[5], 155, fp2))!=NULL)
			{
			opusc(semid, 1);
			strncpy(buf_shm, (char*)tab, 160);
		
			podnies(semid,2);
			}

		fclose(fp2);
		opusc(semid, 1);
		buf_shm[0]='\0';
		podnies(semid, 2);
		}	
		else
		{	char tab[160];
			int valsem;
			usleep(10000);
			printf("\n");
			while(buf_shm[0]!='\0')
			{
				valsem=semctl(semid2, 0, GETVAL, 0);
				
				if((valsem))
				{
				opusc(semid, 2);
				
				strncpy(tab, buf_shm, 160);
				printf("%s",tab);
				}
				
				
				podnies(semid, 1);
				opusc(semid, 2);

				strncpy(tab, buf_shm, 160);
				printf("%s", tab);
					if(valsem)
					{
					podnies(semid, 0);
				
					}
				usleep(50000);		//po to aby pierwszy potomek mógł zmienić semafor po
							//zakończeniu działania
				
				
			}
			semctl(semid, 0, IPC_RMID, &buf3);
			semctl(semid2, 0, IPC_RMID, &buf3);
			shmctl(shmid, IPC_RMID, &buf4);
			printf("Koncze prace\n");
		}
	}

}



return 0;
}
