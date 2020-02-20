#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>  // for INT_MAX

void * slow_printf(void * arg)
{
	const char * msg = arg;

	for(size_t i = 0; i < strlen(msg); i++)
	{
		printf("%c ", msg[i]);
		fflush(stdout);
		sleep(1);
	}

	printf("77");
	fflush(stdout);

	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t h1;
	pthread_t h2;

    char *p;
    long conv;
	int error, nProc;

	if(argc < 2){
		printf("Introduce numero\n");
		exit(EXIT_FAILURE);
	}

    conv = strtol(argv[1], &p, 10);

    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
		printf("Introduce numero\n");
		exit(EXIT_FAILURE);
    }
	
	nProc = conv;
	printf("%d\n", nProc);
	

	printf("El programa %s termino correctamente \n", argv[0]);
}
