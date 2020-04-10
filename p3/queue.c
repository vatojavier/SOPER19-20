/**
 * @file queue.c
 *
 * Implementación de la cola de enteros necesaria en el ejercicio 4
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 7/4/2020
 *
 */
#include <stdlib.h>
#include "queue.h"

struct _Queue {
	int *array;
	unsigned int count, first, last, size;
};

Queue *queue_create(unsigned int size) {
	Queue *newQueue = malloc(sizeof(Queue));
	newQueue->array = malloc(sizeof(int) * size);
	newQueue->size = size;
	newQueue->count = newQueue->first = 0;
	newQueue->last = newQueue->size - 1;

	return newQueue;
}

STATUS queue_destroy(Queue *queue) {
	if (!queue)
		return ERROR;
	free(queue->array);
	free(queue);

	return SUCCESS;
}

STATUS queue_add(Queue *queue, int new) {
	if (!queue || queue_is_full(queue)) return ERROR;
	queue->last = (queue->last + 1) % queue->size;
	queue->array[queue->last] = new;
	queue->count++;

	return SUCCESS;
}

int queue_get(Queue *queue) {
	if (!queue || queue_is_empty(queue)) return ERROR_INT;
	int ret_item = queue->array[queue->first];
	queue->first = (queue->first + 1) % queue->size;
	queue->count--;
	return ret_item;
}

BOOL queue_is_full(Queue *queue) {
	if (queue->count == queue->size) return TRUE;
	return FALSE;
}

BOOL queue_is_empty(Queue *queue) {
	return queue->count == 0;
}

STATUS queue_print(Queue *queue, FILE *fp) {
	if (!queue) return ERROR;
	fprintf(fp, "Queue: < ");

	if (queue_is_empty(queue) == FALSE) {
		for (unsigned int i = queue->first; i != queue->last; i = (i + 1) % queue->size) {
			fprintf(fp, "%i ", queue->array[i]);
		}
		fprintf(fp, "%i ", queue->array[queue->last]);
	}
	fprintf(fp, ">");
	return SUCCESS;
}