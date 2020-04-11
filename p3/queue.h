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

#include "types.h"
#include <stdio.h>


typedef struct _Queue Queue;

Queue *queue_create(unsigned int size);

STATUS queue_destroy(Queue *queue);

STATUS queue_add(Queue *queue, int new);

int queue_get(Queue *queue);

BOOL queue_is_full(Queue *queue);

BOOL queue_is_empty(Queue *queue);

STATUS queue_print(Queue *queue);

#endif //QUEUE_H