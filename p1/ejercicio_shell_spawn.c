/**
 * @file ejercicio_shell.c
 *
 * Ejercicio opcional en el que se simula una shell con procesos usando posix_spawn
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
#include <string.h>
#include <spawn.h>

#define MAX_COM 100

int main(void){
    pid_t pid;

    char comando[MAX_COM];/*Comando introducido en un string*/
    char *comSep[MAX_COM];/*Comando separado en strings*/
    char *token;
    char*   spawnedEnv[1];

    spawnedEnv[0] = NULL;

    int cmdCont = 0;
    int status;
 
    while (1)
    {
        cmdCont = 0;
        printf("\nIntroduce comando: ");
        
        if(fgets(comando,MAX_COM,stdin) == NULL){
            printf("\n");
            exit(0);
        }

        /* get the first token */
        token = strtok(comando, " ");
        token[strcspn(token, "\n")] = '\0'; /*Eliminando el salto de linea*/
        comSep[cmdCont] = token;
        cmdCont++;

        
        while(1) {
            token = strtok(NULL, " ");

            if(token == NULL){
                comSep[cmdCont] = NULL;
                break;
            }

            token[strcspn(token, "\n")] = '\0';

            comSep[cmdCont] = token;
            cmdCont++;
        }

        status = posix_spawnp(&pid, comSep[0], NULL, NULL, comSep, spawnedEnv);
        wait(NULL);
        printf("\nTerminado con status=%d\n", status);

    }
}