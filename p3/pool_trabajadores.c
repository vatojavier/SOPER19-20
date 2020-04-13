/**
 * @file pool_trabajadores.h
 *
 * Implementación de funciones para el ejercicio 6.
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 11/4/2020
 *
 */

#include "pool_trabajadores.h"

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

int senal_todos_hijos(int n_hijos,pid_t *pids, int senial){

    for(int i = 0; i < n_hijos; i++){
        kill(pids[i], senial);
    }

    return 0;
}

//TODO: contar caracteres
int contar_caracter(char *cadena, char caracter){
    int contador = 0;

    for(int i = 0; i < strlen(cadena); i++){
        if(cadena[i] == caracter){
            contador++;
        }
    }

    return contador;
}