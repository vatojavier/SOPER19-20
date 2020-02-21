/**
 * @file pipes.c
 *
 * Implementación de libreria de pipes para el ultimo ejercicio
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 21/2/2020
 */

#include "pipes.h"


int generar_numero(){
    
    srand(time(NULL));
    return random() % 1001;
}

int read_num_de(int *pipe, int *num){

    /* Cierre del descriptor de entrada en el hijo */
    close(pipe[1]);

    /* Leer algo de la tubería*/
    ssize_t nbytes = 0;
    do {
        nbytes = read(pipe[0], num, sizeof(num));
        if(nbytes == -1)
        {
            perror("read padre");
            return -1;
        }
        /*if(nbytes > 0){
            printf("He recibido el string: %d", *num);
        }*/
    } while(nbytes != 0);

    return 0;
}

int write_num_en(int *pipe, int *num){

    /* Cierre del descriptor de salida en el hijo */
    close(pipe[0]);

    ssize_t nbytes = write(pipe[1], num, sizeof(num));
    if(nbytes == -1)
    {
        perror("write");
        return -1;
    }
    return 0;
}