/**
 * @file mq_workers_pool.c
 *
 * Programa que instancia los procesos trabajadores del ejercicio 6
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 11/4/2020
 *
 */

#include "pool_trabajadores.h"

int main(int argc, char **argv){

    if(argc != 4){
        printf("Introduce n trabajadores, nombre de cola y caracter a contar\n");
        exit(EXIT_FAILURE);
    }

    if(argv[2][0] != '/'){
        printf("La cola debe empezar por /\n");
        exit(EXIT_SUCCESS);
    }

    exit(EXIT_SUCCESS);
}