#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 3

int main(void)
{
	pid_t pid;
	for(int i = 0; i < NUM_PROC; i++)
	{
		pid = fork();
		if(pid <  0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if(pid ==  0)
		{
			printf("Hijo PID=%d y mi padre=%d\n", getpid(), getppid()); /*mod ej7b*/
			exit(EXIT_SUCCESS);
		}
		else if(pid >  0)
		{
			printf("Padre %d\n", i);
		}
	}

	while((wait(NULL)) > 0){/*mod ej7e*/
		printf("Hijo terminado\n");
	}
	 
	exit(EXIT_SUCCESS);
}
