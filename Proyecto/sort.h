#ifndef _SORT_H
#define _SORT_H

#include <mqueue.h>
#include <semaphore.h>

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "utils.h"

/* Constants. */
#define MAX_DATA 100000
#define MAX_LEVELS 10
#define MAX_PARTS 512
#define MAX_STRING 1024

#define PLOT_PERIOD 1
#define NO_MID -1

/*nombre de la mem. compartida*/
#define SHM_NAME "/LA_MEMORIA"
/*nombre de la cola*/
#define MQ_NAME "/LA_COLA"

/*Estructura atributos de la cola*/
#define MAX_LONG 2000
/*Mensaje enviado por la cola,es una tarea identificada por el nivel y parte de ese nivel*/
typedef struct {
    int nivel;
    int tarea; //tarea[0] es el nivel y tarea[1] es la parte
} Mq_tarea;

/* Type definitions. */

/* Completed flag for the tasks. */
typedef enum {
    INCOMPLETE,
    SENT,
    PROCESSING,
    COMPLETED
} Completed;

/* Task. */
typedef struct {
    Completed completed;
    int ini;
    int mid; /* si es -1 se utiliza bubble, si no, merge*/
    int end;
} Task;

/* Structure for the sorting problem. */
typedef struct{
    Task tasks[MAX_LEVELS][MAX_PARTS];
    int data[MAX_DATA];
    int delay;
    int n_elements;
    int n_levels;
    int n_processes;
    pid_t ppid;

    /*- NUEVAS COSAS -*/
    sem_t sem; /*El sem para poner a completado las tareas*/

} Sort;

/*La famosa memoria compartida*/
Sort *sort;


/* Prototypes. */

/**
 * Sorts an array using bubble-sort.
 * @method bubble_sort
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  vector      Array with the data.
 * @param  n_elements  Number of elements in the array.
 * @param  delay       Delay for the algorithm.
 * @return             ERROR in case of error, OK otherwise.
 */
Status bubble_sort(int *vector, int n_elements, int delay);

/**
 * Merges two ordered parts of an array keeping the global order.
 * @method merge
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  vector     Array with the data.
 * @param  middle     Division between the first and second parts.
 * @param  n_elements Number of elements in the array.
 * @param  delay      Delay for the algorithm.
 * @return            ERROR in case of error, OK otherwise.
 */
Status merge(int *vector, int middle, int n_elements, int delay);

/**
 * Computes the number of parts (division) for a certain level of the sorting
 * algorithm.
 * @method get_number_parts
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  level            Level of the algorithm.
 * @param  n_levels         Total number of levels in the algorithm.
 * @return                  Number of parts in the level.
 */
int get_number_parts(int level, int n_levels);

/**
 * Initializes the sort structure.
 * @method init_sort
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  file_name   File with the data.
 * @param  sort        Pointer to the sort structure.
 * @param  n_levels    Total number of levels in the algorithm.
 * @param  n_processes Number of processes.
 * @param  delay       Delay for the algorithm.
 * @return             ERROR in case of error, OK otherwise.
 */
Status init_sort(char *file_name, Sort *sort_c, int n_levels, int n_processes, int delay);

/**
 * Checks if a task is ready to be solved.
 * @method check_task_ready
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  sort             Pointer to the sort structure.
 * @param  level            Level of the algorithm.
 * @param  part             Part inside the level.
 * @return                  FALSE if it is not ready, TRUE otherwise.
 */
Bool check_task_ready(Sort *sort, int level, int part);

/**
 * Solves a single task of the sorting algorithm.
 * @method solve_task
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  sort       Pointer to the sort structure.
 * @param  level      Level of the algorithm.
 * @param  part       Part inside the level.
 * @return            ERROR in case of error, OK otherwise.
 */
Status solve_task(Sort *sort, int level, int part);

/**
 * Solves a sorting problem using a single process.
 * @method sort_single_process
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  file_name        File with the data.
 * @param  n_levels         Total number of levels in the algorithm.
 * @param  n_processes      Number of processes.
 * @param  delay            Delay for the algorithm.
 * @return                  ERROR in case of error, OK otherwise.
 */
Status sort_single_process(char *file_name, int n_levels, int n_processes, int delay);

/*-------------- NUEVAS FUNCIONES --------------*/

/*--- MANEJADORES ---*/
void manejador_sigterm(int sig);
void manejador_sigusr1(int sig);

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

void liberar_recursos();

/**
 * Abre, da tamaño y mapea la memoria compartida de las etructura sort
 * @method preparar_mem_comp
 * @date   2020-04-28
 * @author Antonio Javier Casado - Aurora Pérez
 * @return ERROR in case of error, OK otherwise.
 */
Status preparar_mem_comp();


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
int senal_todos_hijos(int n_hijos, int senial);

/**
 * Incializa los nuevos campos añadidos a la mem. compartida, incluido semaforos
 * @method preparar_mem_comp
 * @date   2020-04-28
 * @author Antonio Javier Casado - Aurora Pérez
 * @return ERROR in case of error, OK otherwise.
 */
Status init_sort_multiple();


void trabajador(pid_t ppid);

/*-----------------------------------------------------------------------------------*/
/**
 * Resuelve proble de ordenación con multiples procesos
 * @method sort_single_process
 * @date   2020-04-09
 * @author Teaching team of SOPER
 * @param  file_name        File with the data.
 * @param  n_levels         Total number of levels in the algorithm.
 * @param  n_processes      Number of processes.
 * @param  delay            Delay for the algorithm.
 * @return                  ERROR in case of error, OK otherwise.
 */
Status sort_multiple_process(char *file_name, int n_levels, int n_processes, int delay);

#endif
