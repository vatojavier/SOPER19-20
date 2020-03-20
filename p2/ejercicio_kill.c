/**
 * @file ejercicio_kill.c
 *
 * Ejercicio en el que se matan procesos como en bash.
 * 
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/3/2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main(int argc, char *argv[]) {

    int signal, proceso;

    if(argc < 3){
        printf("Introduce dos argumentos\n");
        exit(EXIT_FAILURE);
    }

    signal = atoi(argv[1] + 1);
    proceso = atoi(argv[2]);

    if(kill(proceso, signal) == -1){
        perror("kill");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}