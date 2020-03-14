/**
 * @file prottemp_mejorado.h
 *
 * Cabecera de funciones para Ejercicio 12 en el que se crean procesos
 * y se mandan señales entre ellos con concurrencia
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 9/4/2020
 *
 */

#ifndef PROTTEM_MEJORADO_H
#define PROTTEM_MEJORADO_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>


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


/***************************************************************
Nombre: valor_semaforo.
Descripcion:
    Devuelve el valor actual del semáforo
Entrada:
	sem_t *sem: semaforo en cuestion.
    char *sem_name: nombre del semaforo.
Salida:
	-1 error, >= 0 valor del semaforo.
************************************************************/
int get_valor_semaforo(sem_t *sem, char* sem_name);


/***************************************************************
Nombre: leer_numeros.
Descripcion:
    Lee los dos numeros de data.txt, abre y cierra FILE
Entrada:
	char* filename: nombre del file
    int *proc_term: procesos terminados (primera linea)
    unsigned long *suma: suma de los procesos terminados (segunda linea)
Salida:
	-1 error, 0 OK.
************************************************************/
int leer_numeros(char* filename, int *proc_term, unsigned long *suma);

#endif //SOPER19_20_PROTTEM_MEJORADO_H
