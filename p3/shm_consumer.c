/**
 * @file shm_consumer.c
 *
 * Ejercicio 4. 
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Perez Lazaro aurora.perzl@estudiante.uam.es
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
#define MEMNAME "/shared_4"


int main(int argc, char **argv){
	Queue *q = NULL;
	int fd_shm = 0;
	sem_t *sem1 = NULL, *sem2 = NULL, *sem3 = NULL;

	int elem = 0;
	int leidos[10]; /*Array que almacenará el numero de veces que sale cada numero*/

	/*Abrir memoria compartida*/
    fd_shm = shm_open(MEMNAME, O_RDWR, 0);
    if (fd_shm == -1) {
        fprintf(stderr, "Error opening the shared memory segment\n");
        return EXIT_FAILURE;
    }

    /*Mapeo de la memoria compartida*/
    q = (Queue *) mmap(NULL, sizeof(q), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if(q == MAP_FAILED){
		fprintf (stderr, "Error mapping the shared memory segment \n");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
	}

	/*Abrir los diferentes semaforos*/
	if((sem1 = sem_open(SEMAFORO1, 0)) == SEM_FAILED){
		perror("sem_open");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
	}
	sem_unlink(SEMAFORO1);

	if((sem2 = sem_open(SEMAFORO2, 0)) == SEM_FAILED){
		perror("sem_open");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		sem_close(sem1);
		exit(EXIT_FAILURE);
	}
	sem_unlink(SEMAFORO2);
	
	if((sem3 = sem_open(SEMAFORO3, 0)) == SEM_FAILED){
		perror("sem_open");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		sem_close(sem1);
		sem_close(sem2);
		exit(EXIT_FAILURE);
	}
	sem_unlink(SEMAFORO3);

	/*Leer de la cola hasta encontrar -1*/
	while(1){
		sem_wait(sem1);
		sem_wait(sem3);
		elem = queue_get(q);
		sem_post(sem3);
		sem_post(sem2);
		
		if(elem == -1){
			break;	
		}
		leidos[elem]++;
	}

	/*Imprimir histograma*/
	printf("---Histograma---\n");
	for (int i = 0; i < 10; ++i){
		printf("Numero:%d Leido:%d\n", i, leidos[i]);
	}


	/*Borrar memoria*/
    munmap(q, sizeof(q));
	shm_unlink(MEMNAME);
	sem_close(sem1);
	sem_close(sem2);
	sem_close(sem3);
	queue_destroy(q);

	exit(EXIT_SUCCESS);

}