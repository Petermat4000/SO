
#define ROZM_KANALU 256
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

static struct sembuf buf;

struct komunikat1_2
{
long typ;
char linijka[ROZM_KANALU];
};

struct komunikat2_3
{
long typ;
int dlugosc;
};

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


