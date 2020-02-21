/**
 * @file ejercicio_pipes.c
 *
 * Ejercicio 15 comunicacion entre hijos y padre
 * 
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 21/2/2020
 *
 */
#include "pipes.h"

int main(void) {
    int fd[2];
    pid_t pid;

    int n_aleatorio;


    int pipe_status = pipe(fd);
	if(pipe_status == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

    pid = fork();

    /*--- HIJO 1 ---*/
    if(pid < 0){
        perror("Error en fork 1\n");
		exit(EXIT_FAILURE);
    }else if (pid == 0){

        printf("Soy el primer hijo\n");
        n_aleatorio = generar_numero();
        printf("He generado el num %d\n", n_aleatorio);
        
        exit(EXIT_SUCCESS);
    }

    pid = fork();

    /*--- HIJO 2 ---*/
    if(pid < 0){
        perror("Error en fork 1\n");
		exit(EXIT_FAILURE);
    }else if (pid == 0){
        printf("Soy el segundo hijo\n");
        exit(EXIT_SUCCESS);
    }

    while (wait(NULL) > 0)
    {
       printf("Un hijo terminado\n");
    }
    
    printf("padre terminando!!!\n");
	/*Para ej 15 cerrar las tuberias no utilizadas en cada proceso, ej1 cierra tuberia2 completamente*/
}
