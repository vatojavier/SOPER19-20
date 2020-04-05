/**
 * @file concurrence.c
 *
 * Cabecera de funciones para Ejercicio 3. Memoria compartida con diferentes procesos malamente.
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author
 * @date 5/4/2020
 *
 */

#ifndef CONCURRENCE_H
#define CONCURRENCE_H

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

/***************************************************************
Nombre: armar_manejador.
Descripcion:
    Arma un manejador de una señal.
Entrada:
	struct sigaction act: La estructura.
	int signal: señal a manejar.
	void (*fun_ptr)(int): ptro a función manjadora.
Salida:
	-1 error o 0 ok.
************************************************************/
int armar_manejador(struct sigaction *act, int signal, void (*fun_ptr)(int));

#endif //CONCURRENCE_H
