/**
 * @file shm_concurrence_solved.c
 *
 * Ejercicio 3.c. Memoria compartida con diferentes procesos correcto.
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora López Lázaro
 * @date 5/4/2020
 *
 */

#include "concurrence.h"

#define SHM_NAME "/LA_MEMORIA"
#define MAX_MSG 100

/***************************************************************
Nombre: getMilClock.
Descripcion:
    Funcion dada sin documentacion...
Entrada:
Salida:
************************************************************/
static void getMilClock(char *buf) {
    int millisec;
    char aux[100];
    struct tm* tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }
    tm_info = localtime(&tv.tv_sec);
    strftime(aux, 10, "%H:%M:%S", tm_info);
    sprintf(buf, "%s.%03d", aux, millisec);
}

typedef struct {
    pid_t processid;       /* Logger process PID */
    long logid;            /* Id of current log line */
    char logtext[MAX_MSG]; /* Log text */
    sem_t mutex;           /*Semaforo, ej3.c*/
} ClientLog;

ClientLog *shm_struct;

/***************************************************************
Nombre: manejador.
Descripcion:
    manejador de la señal que viene de los hijos
Entrada:
    int sig: señal
Salida:
************************************************************/
void manejador (int sig) {
    if (sig == SIGUSR1) {
        printf ("Log %ld: Pid %d: %s\n",shm_struct->logid, shm_struct->processid, shm_struct->logtext);
    }
}

int main(int argc, char **argv){

    struct sigaction act_padre;
    pid_t pid;
    char msg[100];
    char cosa_reloj[50];
    int N; //numero procesos hijos
    int M;
    int ret;

    if(argc !=3 ){
        printf("Introduce 2 argumentos\n");
        exit(EXIT_FAILURE);
    }

    N = (int) strtol(argv[1], NULL, 10);
    M = (int) strtol(argv[2], NULL, 10);

    //Manejador del padre
    armar_manejador(&act_padre, SIGUSR1, &manejador);

    /*---CREANDO MEMORIA ---*/
    /* We create the shared memory */
    int fd_shm = shm_open(SHM_NAME,
                          O_RDWR | O_CREAT | O_EXCL, /* Create it and open for reading and writing */
                          S_IRUSR | S_IWUSR); /* The current user can read and write */

    if (fd_shm == -1) {
        fprintf(stderr, "Error creating the shared memory segment\n");
        return EXIT_FAILURE;
    }

    /* Resize the memory segment */
    if (ftruncate(fd_shm, sizeof(ClientLog)) == -1) {
        fprintf(stderr, "Error resizing the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    /* Map the memory segment HIJOS LO HEREDAN*/
    shm_struct = mmap(NULL,
                      sizeof(*shm_struct), /* Memory mapping size */
                      PROT_READ | PROT_WRITE, /* Read and write */
                      MAP_SHARED, /* Share data changes */
                      fd_shm, /* File or mem_file descriptor */
                      0);
    close(fd_shm);

    if (shm_struct == MAP_FAILED) {
        fprintf(stderr, "Error mapping the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }
    /*---FIN CREANDO MEMORIA ---*/

    /* Initialize the memory */
    shm_struct->logid = -1;
    memcpy(shm_struct->logtext, "nada", sizeof("nada"));
    shm_struct->processid = 0;

    ret = sem_init(&shm_struct->mutex, 1, 1);
    if(ret == -1){
        perror("sem_init");
        /* Free the shared memory */
        munmap(shm_struct, sizeof(*shm_struct));
        shm_unlink(SHM_NAME);

        exit(EXIT_FAILURE);
    }

    srand(time(0));

    for(int i = 0; i < N; i++){

        pid = fork();

        if(pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }else if(pid == 0){

            for(int j = 0; j < M; j++){

                int t = 100000 + (rand() % 910000);

                getMilClock(cosa_reloj);
                sprintf(msg, "Soy el proceso %d a las %s\n", getpid(), cosa_reloj);

                usleep(t);

                sem_wait(&shm_struct->mutex);
                    shm_struct->processid = getpid();
                    if(shm_struct->logid == -1){
                        shm_struct->logid = 0;
                    }
                    shm_struct->logid ++;
                    memcpy(shm_struct->logtext, msg, sizeof(msg));

                    kill(getppid(), SIGUSR1);
                sem_post(&shm_struct->mutex);
            }

            exit(EXIT_SUCCESS);
        }
    }

    /*Padre*/
    while(1){
        sleep(999);

        if(shm_struct->logid == N*M){ //Si ha sido el ultimo se termina
            break;
        }
    }

    /* Free the shared memory */
    munmap(shm_struct, sizeof(*shm_struct));
    shm_unlink(SHM_NAME);

    exit(EXIT_SUCCESS);
}