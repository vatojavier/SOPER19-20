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
	long res = 0;

	for(int i = 0; i < (pid/10); i++){
		res = i + res;
	}

	return res;
}

int armar_manejador(struct sigaction* act, int signal, void (*fun_ptr)(int)){

	if (signal < 0 || fun_ptr == NULL){
		return -1;
	}

	act->sa_handler = fun_ptr;
	sigemptyset(&(act->sa_mask));
	act->sa_flags = 0;
	if (sigaction(signal, act, NULL) < 0) {
		perror("Armar senial:");
		return -1;
	}

	return 0;

}
