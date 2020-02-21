/**
 * @file hilos.h
 *
 * Cabecera de la libreria de los hilos para el ejercicio_hilos
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/2/2020
 */

#ifndef HILOS_H
#define HILOS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <time.h>


/*Estructura que contiene los argumentos para los threads*/
typedef struct _Args Args;
struct _Args{
	int tEspera;
	int x; 
};


/***************************************************************
Nombre: pasarAint.
Descripcion: 
    Pasa string a int
Entrada:
    char *argv: string para pasar a entero
Salida:
    int: > 0 si todo fue correcto, -1 en caso de error.
************************************************************/
int pasarAint(char *argv);

/***************************************************************
Nombre: setParametros.
Descripcion: 
    Inicializa los parametros para la funcion de pthread_create
Entrada:
    Args* structArg: puntero a estructura
    int num: numero del hilo creado
Salida:
    int: > 0 si todo fue correcto, -1 en caso de error.
************************************************************/
int setParametros(Args* structArg, int num);


/***************************************************************
Nombre: esperarYcalcular.
Descripcion: 
    Espera un tiempo aleatorio y calcula el cubo de x
Entrada:
    void * arg: struct con los argumentos
Salida:
************************************************************/
void* esperarYcalcular(void * arg);


#endif