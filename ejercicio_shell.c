/*P1 Ejercicio 10*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COM 100

int main(void){
    pid_t pid;

    char comando[MAX_COM];/*Comando introducido en un string*/
    char *comSep[MAX_COM];/*Comando separado en strings*/
    char *token;

    int cmdCont = 0;

 
    while (1)
    {
        cmdCont = 0;
        printf("Introduce comando: ");
        
        if(fgets(comando,MAX_COM,stdin) == NULL){
            printf("\n");
            exit(0);
        }

        /* get the first token */
        token = strtok(comando, " ");
        printf("%s\n", token);
        printf("%ld\n", strlen(token));
        token[strcspn(token, "\n")] = '\0'; /*Eliminando el salto de linea*/
        comSep[cmdCont] = token;
        cmdCont++;
        printf("%s\n", token);
        printf("%ld\n", strlen(token));

        
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

        

        pid = fork();

        if(pid < 0){
            perror("fork");
			exit(EXIT_FAILURE);
        }else if( pid == 0){
            printf("Ejecutando %s\n",comSep[0]);
            execvp(comSep[0], comSep);
            exit(EXIT_SUCCESS);
        }else if (pid > 0){
            wait(NULL);
        }

        
    }
    

}