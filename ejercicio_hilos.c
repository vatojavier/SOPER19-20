#include "hilos.h"

int main(int argc, char *argv[]) {
	pthread_t h1;
	pthread_t h2;

	int error, nProc;

	if(argc < 2){
		printf("Introduce numero\n");
		exit(EXIT_FAILURE);
	}
	
	nProc = pasarAint(argv[1]);
	if(nProc < 0){
		printf("Introduce numero\n");
		exit(EXIT_FAILURE);
	}
	printf("%d\n", nProc);
	

	printf("El programa %s termino correctamente \n", argv[0]);
}
