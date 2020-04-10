/**
 * @file shm_producer.c
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
	int fd_shm = 0;
	Queue *q = NULL;
	sem_t *sem1 = NULL, *sem2 = NULL, *sem3 = NULL;

	int n = atoi(argv[1]);
	int aleat = atoi(argv[2]);
	int num_aleat = 0;

	/*Control de errores con los argumentos*/
	if(argc != 3 || n < 0 || (aleat != 0 && aleat != 1)){
        printf("Error en los argumentos: ejecutable N Aleat\n");
        exit(EXIT_FAILURE);
    }

    /*Creación de la memoria compartida*/
	fd_shm = shm_open (MEMNAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd_shm == -1) {
        fprintf(stderr, "Error creating the shared memory segment\n");
        return EXIT_FAILURE;
    }else {
        printf ("Shared memory segment created\n");
    }

    /*Mapeo de la memoria compartida*/
    q = (Queue *) mmap(NULL, sizeof(q), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if(q == MAP_FAILED){
		fprintf (stderr, "Error mapping the shared memory segment \n");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
	}

	/*Inicializamos la cola*/
	q = queue_create(n);

	/*Creamos semáforos*/
	/*Ira incrementando hasta llegar al estado lleno*/
	if ((sem1 = sem_open(SEMAFORO1, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
		perror("sem_open");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
	}
	sem_unlink(SEMAFORO1);
	
	/*Ira decrementando hasta llegar al estado vacio*/
	if((sem2 = sem_open(SEMAFORO2, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, n)) == SEM_FAILED){
		perror("sem_open");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		sem_close(sem1);
		exit(EXIT_FAILURE);
	}
	sem_unlink(SEMAFORO2);
	
	/*Se encarga de controlar la zona de insertar elementos en la cola*/
	if((sem3 = sem_open(SEMAFORO3, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED){
		perror("sem_open");
		munmap(q, sizeof(q));
		shm_unlink(MEMNAME);
		sem_close(sem1);
		sem_close(sem2);
		exit(EXIT_FAILURE);
	}
	sem_unlink(SEMAFORO3);


    /*generar N numeros aleatorios e inyectar en cola*/
    for (int i = 0; i < n; ++i){
    	if(aleat == 0){ /*Numeros aleatorios*/
    		num_aleat = rand() % 10;
    	}else if(aleat == 1){/*Numeros en secuencia*/
    		num_aleat = i % 10;
    	}

    	sem_wait(sem2);
		sem_wait(sem3);

		/*Insertar en la cola*/
		queue_add(q, num_aleat);

		sem_post(sem3);
		sem_post(sem1);

    }

    /*Insertar -1 al final de la cola*/
    queue_add(q, -1);
    sem_post(sem1);

    /*Borrar memoria*/
    munmap(q, sizeof(q));
	shm_unlink(MEMNAME);
	sem_close(sem1);
	sem_close(sem2);
	sem_close(sem3);
	
	exit(EXIT_SUCCESS);

}

