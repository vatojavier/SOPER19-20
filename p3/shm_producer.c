/**
 * @file shm_producer.c
 *
 * Ejercicio 4. Productor con memoria compartida
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
#define MEMNAME "/ej4shared"


int main(int argc, char **argv){
	int fd_shm = 0, error = 0;
	Sem *sem;

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

    error = ftruncate(fd_shm, sizeof(Sem));
	if(error == -1){
		fprintf (stderr, "Error resizing the shared memory segment \n");
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
	}

    /*Mapeo de la memoria compartida*/
    sem = (Sem *) mmap(NULL, sizeof(Sem), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    close(fd_shm);
	if(sem == MAP_FAILED){
		fprintf (stderr, "Error mapping the shared memory segment \n");
		munmap(sem, sizeof(*sem));
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
	}

    /*Inicializamos la cola*/
	sem->q.front = 0;
	sem->q.rear = -1;
	for (int i = 0; i < MAX_ELEM; ++i)
	{
		sem->q.elementos[i] = 0;
	}

	sem->size = n;

	/*Creamos semáforos*/
	/*Ira incrementando hasta llegar al estado lleno*/
	if (sem_init(&sem->sem1, 1, 0) ==-1) {
        perror("sem_init");
        munmap(sem, sizeof(*sem));
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
    }
	sem_unlink(SEMAFORO1);

	/*Ira decrementando hasta llegar al estado vacio*/
    if (sem_init(&sem->sem2, 1, MAX_ELEM) == -1) {
        perror("sem_init");
        sem_destroy(&sem->sem1);
        munmap(sem, sizeof(*sem));
		shm_unlink(MEMNAME);
		exit(EXIT_FAILURE);
    }
	sem_unlink(SEMAFORO2);

	/*Se encarga de controlar la zona de insertar elementos en la cola*/
    if (sem_init(&sem->sem3, 1, 1) ==-1) {
        perror("sem_init");
        sem_destroy(&sem->sem1);
        sem_destroy(&sem->sem2);
        munmap(sem, sizeof(*sem));
		shm_unlink(MEMNAME);
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

    	sem_wait(&sem->sem2);
		sem_wait(&sem->sem3);

		/*Insertar en la cola*/
		sem->q.elementos[sem->q.rear] = num_aleat;
  		sem->q.rear = (sem->q.rear + 1) % MAX_ELEM;
		printf("Insertado %d\n", num_aleat);
		

		sem_post(&sem->sem3);
		sem_post(&sem->sem1);

    }

    /*Imprimimos la cola*/
    printf("Queue < ");
	for(int i = 0; i < sem->size; i++){
		printf("%d ", sem->q.elementos[i]);
	}
	printf(">\n");

    /*Borrar memoria*/
    munmap(sem, sizeof(*sem));
    sem_destroy(&sem->sem3);
  	sem_destroy(&sem->sem2);
  	sem_destroy(&sem->sem1);
	
	exit(EXIT_SUCCESS);

}

