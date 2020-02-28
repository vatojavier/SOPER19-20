/**
 * @file prottemp.c
 *
 * Implementación de funciones para prottemp
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/3/2020
 *
 */

#include "prottemp.h"

int senal_todos_hijos(int n_hijos,pid_t *pids, int senial){

	for(int i = 0; i < n_hijos; i++){
		kill(pids[i], senial);
	}

	return 0;
}


long sumar_numeros(){
	pid_t pid = getpid();
	long res = 1;

	for(int i = 0; i < (pid/10); i++){
		res = i + res;
	}

	return res;
}
