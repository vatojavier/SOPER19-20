/**
 * @file ejercicio_arbol.c
 *
 * Ejercicio en el que se crea procesos tal y como se indica en el ej8.
 * 
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/2/2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define NUM_PROC 3

int main(void){
    pid_t pid;

    for (int i = 0; i < NUM_PROC; i++){

        pid = fork();

        if( pid < 0){
            perror("fork");
			exit(EXIT_FAILURE);
        }else if ( pid == 0){
            printf("Hijo %d\n", getpid());
        }else if( pid > 0 ){
            printf("Padre \n");
            wait(NULL);
            exit(EXIT_SUCCESS);
        }
        wait(NULL);       

    }

    printf("Soy el ultimo hijo\n");
    exit(EXIT_SUCCESS);
}