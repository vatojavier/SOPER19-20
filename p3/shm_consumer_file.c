/**
 * @file shm_consumer.c
 *
 * Ejercicio 4. Consumidor
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Perez Lazaro aurora.perezl@estudiante.uam.es
 * @date 7/4/2020
 *
 */

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

#include "queue.h"

#define SEMAFORO1 "/sem1"
#define SEMAFORO2 "/sem2"
#define SEMAFORO3 "/sem3"
#define FNAME "file4.txt"

typedef struct _Sem {
    sem_t sem1;
    sem_t sem2;
    sem_t sem3;
    Queue *q;
    int size;
} Sem;

int main(int argc, char **argv){
	Sem *sem;
	int pf = 0;
	int elem = 0;
	int leidos[10]; /*Array que almacenará el numero de veces que sale cada numero*/

	/*Abrir fichero*/
    pf = open(FNAME, O_RDWR, 0);
    if (pf == -1) {
        fprintf (stderr, "Error opening the file \n");
        exit(EXIT_FAILURE);
    }

    /*Mapeo de la memoria compartida*/
    sem = mmap(NULL, sizeof(Sem), PROT_READ | PROT_WRITE, MAP_SHARED, pf, 0);
	if(sem == MAP_FAILED){
		fprintf (stderr, "Error mapping the shared memory segment \n");
		munmap(sem, sizeof(*sem));
		shm_unlink(FNAME);
		exit(EXIT_FAILURE);
	}

	/*Leer de la cola hasta encontrar -1*/
	while(1)
	{
		sem_wait(&sem->sem1);
		sem_wait(&sem->sem3);
		elem = queue_get(sem->q);
		sem_post(&sem->sem3);
		sem_post(&sem->sem2);
		if(elem == -1)
			break;
		leidos[elem]++;
	}

	/*Imprimir histograma*/
	printf("---Histograma---\n");
	for (int i = 0; i < 10; ++i){
		printf("Numero:%d Leido:%d\n", i, leidos[i]);
	}


	/*Borrar memoria*/
	queue_destroy(sem->q);
    munmap(sem, sizeof(*sem));
	shm_unlink(FNAME);

	exit(EXIT_SUCCESS);

}