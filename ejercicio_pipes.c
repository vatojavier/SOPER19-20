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
    int fd1[2];
    int fd2[2];
    pid_t pid;
    int pipe_status1, pipe_status2, ret;

    int n_aleatorio; /*Creado por hijo1*/
    int n_rec_padre; /*Recibido por el padre*/
    int n_rec_hijo;  /*Recibio por el hijo*/


    pipe_status1 = pipe(fd1);
    pipe_status2 = pipe(fd2);
	if(pipe_status1 == -1 || pipe_status2 == -1)
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

        /*cerrar tuberia2 completa*/
        close(fd2[0]);
        close(fd2[1]);

        n_aleatorio = generar_numero();
        printf("Soy primer hijo, he generado el num %d\n", n_aleatorio);

        ret = write_num_en(fd1, &n_aleatorio);
        if(ret == -1){
            printf("Error al escribir\n");
            exit(EXIT_FAILURE);
        }

		printf("He escrito en el pipe\n");

        exit(EXIT_SUCCESS);
    }

    pid = fork();

    /*--- HIJO 2 ---*/
    if(pid < 0){
        perror("Error en fork 1\n");
		exit(EXIT_FAILURE);
    }else if (pid == 0){
        /*Cierre tubieria 1 completa*/
        close(fd1[0]);
        close(fd1[1]);

        /* Leer algo de la tubería*/
        ret = read_num_de(fd2, &n_rec_hijo);
        if(ret == -1){
            printf("Error al leer en hijo 2\n");
            exit(EXIT_FAILURE);
        }

        printf("Soy hijo 2 he recibido %d\n", n_rec_hijo);

        exit(EXIT_SUCCESS);
    }

    /*--- PADRE ---*/

    /*Read de hijo 1*/
    ret = read_num_de(fd1, &n_rec_padre);
    if(ret == -1){
        printf("Error al leer en padre\n");
        exit(EXIT_FAILURE);
    }

    printf("Soy padre, recibido %d\n", n_rec_padre);

    close(fd1[0]);
    close(fd1[1]);

    printf("Voy a escribir a hijo\n");
    /*Write a hijo 2*/
    ret = write_num_en(fd2, &n_rec_padre);
    if(ret == -1){
        printf("Error al escribir en padre a hijo 2\n");
        exit(EXIT_FAILURE);
    }

    while (wait(NULL) > 0)
    {
       printf("Un hijo terminado\n");
    }
     
    printf("padre terminando!!!\n");
	/*Para ej 15 cerrar las tuberias no utilizadas en cada proceso, ej1 cierra tuberia2 completamente*/
}
