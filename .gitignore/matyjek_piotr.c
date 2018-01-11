#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>


int main(void)
{
char tablica[30];
int pid, i, pid2, pidexec=2, pidparent;

pidparent=getpid();

if((pid=fork())<0) printf("blad1\n");
else
{
	if(pid==0)
	{
	
	pid2=fork();
		for(i=0;i<2;i++)
		{
			if(pid2!=0)
			{
			pid2=fork();
			}
		}
	}
	else
	{
	if((pidexec=fork())<0) printf("blad2\n");
	else
	{
		if(pidexec==0)
		{
		pid=fork();
		}
	}
	}
}

if(pidparent==getpid())
{
	if((pid=fork())<0) printf("blad4");
	else
	{
	if(pid==0)
	{
		fork();
		fork();
	}}
}

printf("%d\n", getpid());
sleep(3);

if(pidexec==0 && pidparent==getppid())
{

sprintf(tablica, "%d", pidparent);
execlp("pstree","pstree", "-c", tablica, (char *) 0);

}
else
{sleep(5);}
getchar();

return 0;
}
		  
