/**
 * @file shm_concurrence.c
 *
 * Ejercicio 3. Memoria compartida con diferentes procesos malamente.
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author
 * @date 5/4/2020
 *
 */

#include "concurrence.h"

#define SHM_NAME "/LA_MEMORIA"

static volatile sig_atomic_t got_signal_USR1 = 0;

/***************************************************************
Nombre: manejador_USR1.
Descripcion:
    manejador de la señal que viene de los hijos
Entrada:
    int sig: señal
Salida:
************************************************************/
void manejador_USR1(int sig){
    got_signal_USR1 ++;
}

typedef struct{
    pid_t processid; /* Logger process PID DEL ULTIMO PROCESO QUE ACCEDE??¿?¿*/
    long logid; /* Id of current log line ASUMIRÉ QUE ES EL NUM DE PROCESOS QUE ACCEDEN*/
    char logtext [2000]; /* Log text */
} ClientInfo;

int main(int argc, char **argv){

    struct sigaction act_padre;
    pid_t pid;
    int N; //numero procesos hijos
    int M;



    if(argc !=3 ){
        printf("Introduce 2 argumentos\n");
        exit(EXIT_FAILURE);
    }

    N = (int) strtol(argv[1], NULL, 10);
    M = (int) strtol(argv[2], NULL, 10);

    //Manejador del padre
    armar_manejador(&act_padre, SIGUSR1, &manejador_USR1);

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
    if (ftruncate(fd_shm, sizeof(ClientInfo)) == -1) {
        fprintf(stderr, "Error resizing the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    /* Map the memory segment HIJOS LO HEREDAN*/
    ClientInfo *example_struct = mmap(NULL,
                                            sizeof(*example_struct), /* Memory mapping size */
                                            PROT_READ | PROT_WRITE, /* Read and write */
                                            MAP_SHARED, /* Share data changes */
                                            fd_shm, /* File or mem_file descriptor */
                                            0);
    close(fd_shm);

    if (example_struct == MAP_FAILED) {
        fprintf(stderr, "Error mapping the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }
    /*---FIN CREANDO MEMORIA ---*/

    /* Initialize the memory */
    example_struct->logid = -1;
    memcpy(example_struct->logtext, "nada", sizeof("nada"));
    example_struct->processid = 0;

    srand(time(0));

    for(int i = 0; i < N; i++){

        pid = fork();

        if(pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }else if(pid == 0){

            for(int j = 0; j < M; j++){
                int t = 100000 + (rand() % 910000);
                printf("hijo durmiendo %d tiempo\n", t);
                usleep(t);

                example_struct->processid = getpid();
                example_struct->logid ++;


                kill(getppid(), SIGUSR1);
            }

            exit(EXIT_SUCCESS);
        }

    }

    /*Padre*/
    while(1){ //Espera ma o meno activa :(
        sleep(999);
        if(got_signal_USR1){

            printf("%ld:%d:%s\n", example_struct->logid, example_struct->processid, example_struct->logtext);

            if(example_struct->logid == N*M -1){ //Si ha sido el ultimo se termina
                break;
            }

        }
    }

    /* Free the shared memory */
    munmap(example_struct, sizeof(*example_struct));
    shm_unlink(SHM_NAME);

    exit(EXIT_SUCCESS);
}