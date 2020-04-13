/**
 * @file shm_consumer.c
 *
 * Ejercicio 4. Consumidor con el uso de ficheros
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Perez Lazaro aurora.perezl@estudiante.uam.es
 * @date 7/4/2020
 *
 */
#include "queue.h"

#define SEMAFORO1 "/sem1"
#define SEMAFORO2 "/sem2"
#define SEMAFORO3 "/sem3"
#define FNAME "file4b.txt"


int main(int argc, char **argv){
	Sem *sem;
	int pf = 0;
	int elem = 0;
	int leidos[10]; /*Array que almacenará el numero de veces que sale cada numero*/


	for (int i = 0; i < 10; ++i)
	{
		leidos[i] = 0;
	}
	
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

	/*Imprimimos la cola*/
    printf("Queue < ");
	for(int i = 0; i < sem->size; i++){
		printf("%d ", sem->q.elementos[i]);
	}
	printf(">\n");
  	
	for (int i = 0; i < sem->size; ++i)
	{
		sem_wait(&sem->sem1);
		sem_wait(&sem->sem3);
		elem = sem->q.elementos[sem->q.front];
		sem->q.elementos[sem->q.front] = 0;
        sem->q.front = (sem->q.front + 1) % MAX_ELEM;
		sem_post(&sem->sem3);
		sem_post(&sem->sem2);
		leidos[elem]++;
	}

	/*Imprimir histograma*/
	printf("---Histograma---\n");
	for (int i = 0; i < 10; ++i){
		printf("Numero:%d Leido:%d\n", i, leidos[i]);
	}


	/*Borrar memoria*/
    munmap(sem, sizeof(*sem));
	unlink(FNAME);

	exit(EXIT_SUCCESS);

}