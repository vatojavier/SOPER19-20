/**
 * @file concurrence.c
 *
 * Implementación de funciones para Ejercicio 3. Memoria compartida con diferentes procesos malamente.
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author
 * @date 5/4/2020
 *
 */

#include "concurrence.h"

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