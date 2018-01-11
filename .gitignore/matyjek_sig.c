#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>



int sen1=0;
int sen2=0;

void obsluga (int sig)
{
char c;
int filedesc;

		if((filedesc=open("bufor.txt",O_RDONLY))<0)
		{
		printf("error filedesc1-potomny\n");
		exit(2);
		}
		lseek(filedesc,0,SEEK_SET);


		while((read(filedesc, &c, 1))>0)
		{
		printf("%c",c);
		}

		if((close(filedesc))!=0)
		{
		printf("problem z zamknieciem filedesc (proces pot)\n");
		exit(2);
		}


		sen2=0;
	

}

void obslugaUsun(int sig)
{
	if(unlink("/home/piotr/Dokumenty/bufor.txt"))
	{
	printf("blad usuniecia pliku\n");
	exit(3);
	}
exit(0);

}

void obsluga2 (int sig)
{


sen1=0;

}

void unstop (int sig)
{
raise(SIGCONT);
}

void obslugaNIC (int sig)
{
raise(SIGCONT);
}






int main(void)
{
FILE *fp;

int pid, pidparent, filedesc;
sigset_t signalSet;
sigset_t signalSet2;



if((pid=fork())<0)
{
printf("cos nie wyszlo\n");
exit(1);

}
else
{
	if(pid==0)
	{
	
	//sigset_t *signalSet;
	if(sigemptyset(&signalSet))printf("error!\n");
	if(sigfillset(&signalSet))printf("errrrrrr!\n");
	if(sigdelset(&signalSet,SIGUSR1))printf("error!\n");
	if(sigdelset(&signalSet,SIGUSR2))printf("error!\n");
	sigdelset(&signalSet,SIGCONT);
	sigprocmask(SIG_SETMASK, &signalSet,NULL);

	pidparent=getppid();


	signal(SIGUSR1,obsluga);
	signal(SIGUSR2,obslugaUsun);
	signal(SIGCONT,obsluga);
	signal(SIGSTOP,unstop);

	sen2=1;
	
		
	
		
	
	
		while(1)
		{
			
			while(sen2)
			{
			pause();
			}
			usleep(10000);
		

			kill(pidparent,SIGUSR1);
			sen2=1;
		}

		



	}
	else
	{

	//sigset_t *signalSet2;
	if(sigemptyset(&signalSet2))printf("error!\n");
	if(sigfillset(&signalSet2))printf("errrrrrr!\n");
	if(sigdelset(&signalSet2,SIGUSR1))printf("error!\n");	
		sigdelset(&signalSet2,SIGUSR2);
		sigdelset(&signalSet2,SIGCONT);
	
	sigprocmask(SIG_SETMASK, &signalSet,NULL);
	int sig1, i=0;
	char c;
	

		signal(SIGUSR1,obsluga2);
		signal(SIGCONT,obsluga2);
		signal(SIGSTOP,unstop);
		signal(SIGUSR2,obslugaNIC);

		if((fp=fopen("/etc/profile","r"))==NULL)
		{
		printf("cos nie teges\n");
		exit(1);

		}
			if((filedesc=open("bufor.txt",O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU))<0)
			{
			printf("error filedesc1\n");
			exit(1);
			}
		


		while(fscanf(fp,"%c",&c)>0)
		{	
			

			
			if((write(filedesc, &c, 1))<0)
			{
			printf("problem z wpisaniem do bufora\n");
			exit(1);
			}

			if(c=='\n')
				{

					if((close(filedesc)))
					{
					printf("problem z zamknieciem filedesc (proces mac)\n");
					exit(1);
					}
			
					if(kill(pid,SIGUSR1))printf("blad wyslania sygnalu\n");
					sen1=1;

					

					while(sen1)
					{
					pause();
					}

					if((filedesc=open("bufor.txt",O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU))<0)
					{
					printf("error filedesc1\n");
					exit(1);
					}

				}

	
		}

		kill(pid,SIGUSR2);
	
	fclose(fp);

	}


}


exit(0);
}
