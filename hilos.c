/**
 * @file hilos.c
 *
 * Implementación de libreria de los hilos para el ejercicio_hilos
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/2/2020
 */

#include "hilos.h"


int pasarAint(char *argv){
    char *p;
    long conv;

    conv = strtol(argv, &p, 10);

    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
		return -1;
    }

    return conv;
}


int setParametros(Args* structArg, int num){

	if(structArg == NULL || num < 0){
		return -1;
	}

	structArg->x = num;
	structArg->tEspera = random() % 11;

	return 1;
}

void* esperarYcalcular(void * arg){
	long res;
	int x = ((Args*) arg)->x;

	printf("\nHilo %ld va a calcular %d al cubo", pthread_self(), x);
	printf("\nVoy a dormir %d\n",((Args*) arg)->tEspera);
	fflush(stdout);
	sleep(((Args*) arg)->tEspera);

	res = pow(x,3);

	pthread_exit((void*)res);
    return NULL;
}