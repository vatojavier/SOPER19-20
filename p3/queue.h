/**
 * @file queue.h
 *
 * Declaración de funciones de la cola de enteros necesaria en el ejercicio 4
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 7/4/2020
 *
 */

#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include "types.h"

#define MAX_ELEM 100

typedef struct _Queue {
  int elementos[MAX_ELEM];  /*!< array de elementos enteros de la cola*/
  int front;				/*!< primer elemento de la cola*/
  int rear;					/*!< ultimo elemento en la cola*/
}Queue;


typedef struct _Sem {
    sem_t sem1;	 /*!< semaforo 1*/
    sem_t sem2;  /*!< semaforo 2*/
    sem_t sem3;	 /*!< semaforo 3*/
    Queue q;	 /*!< cola*/
    int size;	 /*!< tamaño de la cola*/
} Sem;

/**
  @brief Funcion que crea una cola

  @param
  @returns cola creada
*/
Queue* queue_create();

/**
  @brief Funcion que destruye una cola, liberando su memoria

  @param queue cola
  @returns
*/
void queue_destroy(Queue *queue);

/**
  @brief Funcion que aniade un elemento a la cola

  @param queue cola
  @param new nuevo elemento que se quiere insertar
  @returns SUCCESS si todo es correcto o ERROR si hay algún error
*/
STATUS queue_add(Queue *queue, int new);

/**
  @brief Funcion que devuelve un elemento de la cola

  @param queue cola
  @returns ret_item nuevo elemento adquirido de la cola
*/
int queue_get(Queue *queue);

/**
  @brief Funcion que informa de si la cola esta llena

  @param queue cola
  @returns TRUE si esta llena o FALSE si no lo está
*/
BOOL queue_is_full(Queue *queue);

/**
  @brief Funcion que informa de si la cola esta vacía

  @param queue cola
  @returns TRUE si esta vacía o FALSE si no lo está
*/
BOOL queue_is_empty(Queue *queue);
/**
  @brief Funcion que imprime una cola

  @param queue cola
  @returns SUCCESS si todo es correcto o ERROR si hay algún error
*/
STATUS queue_print(Queue *queue);

#endif //QUEUE_H