/**
 * @file shm_producer.c
 *
 * Ejercicio 4. Productor
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
	int pf = 0, error = 0;
	Sem *sem;
	//sem_t *sem1 = NULL, *sem2 = NULL, *sem3 = NULL;

	int n = atoi(argv[1]);
	int aleat = atoi(argv[2]);
	int num_aleat = 0;

	/*Control de errores con los argumentos*/
	if(argc != 3 || n < 0 || (aleat != 0 && aleat != 1)){
        printf("Error en los argumentos: ejecutable N Aleat\n");
        exit(EXIT_FAILURE);
    }

    /*Crear fichero*/
    pf = open(FNAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if(pf == -1){
		fprintf (stderr, "Error creating the file \n");
		exit(EXIT_FAILURE);
	}

    error = ftruncate(pf, sizeof(Sem));
	if(error == -1){
		fprintf (stderr, "Error resizing the shared memory segment \n");
		shm_unlink(FNAME);
		exit(EXIT_FAILURE);
	}

    /*Mapeo de la memoria compartida*/
    sem = (Sem *) mmap(NULL, sizeof(Sem), PROT_READ | PROT_WRITE, MAP_SHARED, pf, 0);
    close(pf);
	if(sem == MAP_FAILED){
		fprintf (stderr, "Error mapping the shared memory segment \n");
		munmap(sem, sizeof(*sem));
		shm_unlink(FNAME);
		exit(EXIT_FAILURE);
	}

    /*Inicializamos la cola*/
	sem->q = queue_create();
	sem->size = n;

	/*Creamos semáforos*/
	/*Ira incrementando hasta llegar al estado lleno*/
	if (sem_init(&sem->sem1, 1, 0) == -1) {
        perror("sem_init");
        munmap(sem, sizeof(*sem));
		shm_unlink(FNAME);
		exit(EXIT_FAILURE);
    }
	sem_unlink(SEMAFORO1);

	/*Ira decrementando hasta llegar al estado vacio*/
    if (sem_init(&sem->sem2, 1, MAX_ELEM) == -1) {
        perror("sem_init");
        sem_destroy(&sem->sem1);
        munmap(sem, sizeof(*sem));
		shm_unlink(FNAME);
		exit(EXIT_FAILURE);
    }
	sem_unlink(SEMAFORO2);

	/*Se encarga de controlar la zona de insertar elementos en la cola*/
    if (sem_init(&sem->sem3, 1, 1) == -1) {
        perror("sem_init");
        sem_destroy(&sem->sem1);
        sem_destroy(&sem->sem2);
        munmap(sem, sizeof(*sem));
		shm_unlink(FNAME);
		exit(EXIT_FAILURE);
    }
	sem_unlink(SEMAFORO3);


/*
	if ((sem1 = sem_open(SEMAFORO1, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
		perror("sem_open");
		munmap(q, sizeof(*q));
		shm_unlink(FNAME);
		exit(EXIT_FAILURE);
	}
	
	if((sem2 = sem_open(SEMAFORO2, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, n)) == SEM_FAILED){
		perror("sem_open");
		munmap(q, sizeof(*q));
		shm_unlink(FNAME);
		sem_close(sem1);
		exit(EXIT_FAILURE);
	}
	
	if((sem3 = sem_open(SEMAFORO3, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED){
		perror("sem_open");
		munmap(q, sizeof(*q));
		shm_unlink(FNAME);
		sem_close(sem1);
		sem_close(sem2);
		exit(EXIT_FAILURE);
	}
*/

    /*generar N numeros aleatorios e inyectar en cola*/
    for (int i = 0; i < n; ++i){
    	if(aleat == 0){ /*Numeros aleatorios*/
    		num_aleat = rand() % 10;
    	}else if(aleat == 1){/*Numeros en secuencia*/
    		num_aleat = i % 10;
    	}

    	sem_wait(&sem->sem2);
		sem_wait(&sem->sem3);

		/*Insertar en la cola*/
		error = queue_add(sem->q, num_aleat);
		if(error == ERROR){
			munmap(sem, sizeof(*sem));
    		shm_unlink(FNAME);
    		exit(EXIT_FAILURE);
		}
		printf("Insertado %d\n", num_aleat);

		sem_post(&sem->sem3);
		sem_post(&sem->sem1);

    }
    sem_wait(&sem->sem2);
	sem_wait(&sem->sem3);
    /*Insertar -1 al final de la cola*/
    queue_add(sem->q, -1);
    sem_post(&sem->sem3);
	sem_post(&sem->sem1);

    queue_print(sem->q);

    /*Borrar memoria*/
    munmap(sem, sizeof(*sem));
    //shm_unlink(FNAME);
	//sem_close(sem1);
	//sem_close(sem2);
	//sem_close(sem3);
	
	exit(EXIT_SUCCESS);

}

