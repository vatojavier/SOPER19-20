/**
 * @file prottemp.h
 *
 * Cabecera de funciones para prottemp
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/3/2020
 *
 */

#ifndef PROTTEMP_H_
#define PROTTEMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



/***************************************************************
Nombre: senal_todos_hijos.
Descripcion:
    Manda señan a todos los hijos
Entrada:
    int n_hijos:Cuantos hijos tiene el padre
    pid_t *pids: Array con los pids de los hijos
    int senial: Señal a mandar
Salida:
	-1 error o 0 ok.
************************************************************/
int senal_todos_hijos(int n_hijos, pid_t *pids, int senial);

/***************************************************************
Nombre: sumar_numeros.
Descripcion:
    Suma numeros desde a 1 hasta pid
Entrada:

Salida:
	resultado
************************************************************/
long sumar_numeros();

#endif /* PROTTEMP_H_ */
