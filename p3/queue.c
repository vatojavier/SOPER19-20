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
#include "queue.h"

/**
  @brief Funcion que crea una cola

  @param
  @returns cola creada
*/
Queue *queue_create() {
	Queue *queue = malloc(sizeof(Queue));
	queue->front = 0;
	queue->rear = -1;
	for (int i = 0; i < MAX_ELEM; ++i)
	{
		queue->elementos[i] = 0;
	}

	return queue;
}

/**
  @brief Funcion que destruye una cola, liberando su memoria

  @param queue cola
  @returns
*/
void queue_destroy(Queue *queue) {
	if (!queue)
		return;
	free(queue);
	return;
}

/**
  @brief Funcion que aniade un elemento a la cola

  @param queue cola
  @param new nuevo elemento que se quiere insertar
  @returns SUCCESS si todo es correcto o ERROR si hay algún error
*/
STATUS queue_add(Queue *queue, int new) {
	if (queue == NULL) return ERROR;
	queue->elementos[queue->rear] = new;
  	queue->rear = (queue->rear + 1) % MAX_ELEM;
  	return SUCCESS;
}

/**
  @brief Funcion que devuelve un elemento de la cola

  @param queue cola
  @returns ret_item nuevo elemento adquirido de la cola
*/
int queue_get(Queue *queue) {
	if (!queue || queue_is_empty(queue))
		return ERROR_INT;
	int ret_item = queue->elementos[queue->front];
	queue->elementos[queue->front] = 0;
	queue->front = (queue->front + 1) % MAX_ELEM;

	return ret_item;
}
/**
  @brief Funcion que informa de si la cola esta vacía

  @param queue cola
  @returns TRUE si esta vacía o FALSE si no lo está
*/
BOOL queue_is_empty(Queue *queue) {
	if(!queue)
		return TRUE;
	if(queue->front == queue->rear)
		return TRUE;
	return FALSE;
}
/**
  @brief Funcion que informa de si la cola esta llena

  @param queue cola
  @returns TRUE si esta llena o FALSE si no lo está
*/
BOOL queue_is_full(Queue *queue) {
	if(!queue)
		return TRUE;
	if(queue->front == (queue->rear + 1) % MAX_ELEM)
		return TRUE;
	return FALSE;
}
/**
  @brief Funcion que imprime una cola

  @param queue cola
  @returns SUCCESS si todo es correcto o ERROR si hay algún error
*/
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

