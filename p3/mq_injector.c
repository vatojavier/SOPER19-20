/**
 * @file mq_injector.c
 *
 * Programa que envia contenido del ejercicio 6
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 11/4/2020
 *
 */

#include "pool_trabajadores.h"

int main(int argc, char **argv){

    if(argc != 3){
        printf("Introduce fichero y nombre de la cola\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}