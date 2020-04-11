/**
 * @file pool_trabajadores.h
 *
 * Cabecera de funciones para el ejercicio 6.
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 11/4/2020
 *
 */

#ifndef TRABAJADORES_H
#define TRABAJADORES_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <mqueue.h>


/***************************************************************
Nombre: contar_caracter.
Descripcion:
    Cuenta los caracteres de una cadena.
Entrada:
	char *cadena: la cadena en la que se cuenta.
    char caracter: el caracter a contar
Salida:
	-1 error o >= 0 num de caracteres contados en cadena
************************************************************/
int contar_caracter(char *cadena, char caracter);

#endif //POOL_TRABAJADORES_H
