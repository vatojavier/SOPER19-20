/*P1 Ejercicio 10*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COM 100

int main(void){

    char comando[MAX_COM];
    char *token;
    char separador[2] = " ";

    while (1)
    {
        printf("Introduce comando: ");
        
        if(fgets(comando,MAX_COM,stdin) == NULL){
            printf("\n");
            exit(0);
        }

        token = strtok(comando, separador);

        printf("Has introducido %s",token);

    }
    

}