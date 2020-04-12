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

void queue_create(Queue *queue) {
	if (!queue){
		return;
	}

	queue->front = -1;
	queue->rear = -1;
	for (int i = 0; i < MAX_ELEM; ++i)
	{
		queue->elementos[i] = 0;
	}

	return;
}

void queue_destroy(Queue *queue) {
	if (!queue)
		return;
	free(queue);
	return;
}

STATUS queue_add(Queue *queue, int new) {
	if (!queue || queue_is_full(queue) ==  TRUE) return ERROR;
	queue->elementos[queue->rear] = new;
  	queue->rear = (queue->rear + 1) % MAX_ELEM;
  	return SUCCESS;
}

int queue_get(Queue *queue) {
	if (!queue || queue_is_empty(queue)) return ERROR_INT;
	int ret_item = queue->elementos[queue->front];
	queue->elementos[queue->front] = 0;
	queue->front = (queue->front + 1) % MAX_ELEM;

	return ret_item;
}

BOOL queue_is_empty(Queue *queue) {
	if(!queue)
		return TRUE;
	if(queue->front == queue->rear)
		return TRUE;
	return FALSE;
}

BOOL queue_is_full(Queue *queue) {
	if(!queue)
		return TRUE;
	if(queue->front == (queue->rear + 1) % MAX_ELEM)
		return TRUE;
	return FALSE;
}

int queue_size(const Queue *queue){
  if(!queue){
    return -1;
  }else if(queue->front <= queue->rear){
    return (queue->rear - queue->front);
  }else{
    return (MAX_ELEM - (queue->front - queue->rear));
  }
}

STATUS queue_print(Queue *queue){

  if(!queue) 
  	return -1;
  printf("Queue < ");
  for(int i = queue->front; i < queue->rear; i++){
   	printf("%d ", queue->elementos[i]);
  }
  printf(">\n");
  return SUCCESS;
}

