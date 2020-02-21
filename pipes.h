/**
 * @file pipes.h
 *
 * Cabecera de la libreria de pipes para el último ejercicio
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/2/2020
 */
#ifndef PIPES_H
#define PIPES_H

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>


/***************************************************************
Nombre: generar_numero.
Descripcion: 
    Genera num aleatorio
Entrada:
    
Salida:
    int: aleatorio
************************************************************/
int generar_numero();


/***************************************************************
Nombre: read_num_de.
Descripcion: 
    Lee de un pipe un numero
Entrada:
    int *pipe: el pipe a utilizar
    int *num: puntero a el numero donde se guarda lo recibido
Salida:
    int: -1 si error, 0 si exito
************************************************************/
int read_num_de(int *pipe, int *num);


/***************************************************************
Nombre: write_num_de.
Descripcion: 
    Escribe en un pipe un numero
Entrada:
    int *pipe: el pipe a utilizar
    int *num: puntero a el numero a escribir
Salida:
    int: -1 si error, 0 si éxito
************************************************************/
int write_num_en(int *pipe, int *num);

#endif