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


/***************************************************************
Nombre: generar_numero.
Descripcion: 
    Genera num aleatorio
Entrada:
    
Salida:
    int: aleatorio
************************************************************/
int generar_numero();

#endif