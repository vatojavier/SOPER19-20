/**
 * @file sort_multiple.c
 *
 * Implementación de las funciones de ordenación para multiple procesos
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 28/4/2020
 *
 */

#include "sort.h"

/*Attributes de la cola, si lo pongo en el .h peta*/
struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_curmsgs = 0,
        .mq_msgsize = sizeof(int) //* MAX_LONG //2kB
};

Status preparar_mem_comp(){

    /* We create the shared memory */
    int fd_shm = shm_open(SHM_NAME,
                          O_RDWR | O_CREAT | O_EXCL, /* Create it and open for reading and writing */
                          S_IRUSR | S_IWUSR); /* The current user can read and write */

    if (fd_shm == -1) {
        fprintf(stderr, "Error creating the shared memory segment, puede que esté ya abierta\n");
        return ERROR;
    }

    /* Resize the memory segment */
    if (ftruncate(fd_shm, sizeof(Sort)) == -1) {
        fprintf(stderr, "Error resizing the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return ERROR;
    }

    /* Map the memory segment HIJOS LO HEREDAN*/
    sort = mmap(NULL,
                sizeof(*sort), /* Memory mapping size */
                PROT_READ | PROT_WRITE, /* Read and write */
                MAP_SHARED, /* Share data changes */
                fd_shm, /* File or mem_file descriptor */
                0);

    close(fd_shm);

    if (sort == MAP_FAILED) {
        fprintf(stderr, "Error mapping the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return ERROR;
    }

    return OK;
}

/*#######___ FUNCION PRINCIPAL ___#######*/
Status sort_multiple_process(char *file_name, int n_levels, int n_processes, int delay){
    int i=0, j=0;
    pid_t pid;

    /*Memoria copartida*/
    if(preparar_mem_comp() == ERROR){
        fprintf(stderr, "Error creando mem comp\n");
        return ERROR;
    }

    /*Creando colita*/
    mqd_t queue = mq_open(MQ_NAME,
                          O_CREAT | O_WRONLY, /* This process is only going to send messages */
                          S_IRUSR | S_IWUSR, /* The user can read and write */
                          &attributes);

    if (queue == (mqd_t)-1) {
        perror("mq_open-padre");
        fprintf(stderr, "Error opening the queue - proceso padre - borrar de /dev/mqueue\n");
        return ERROR;
    }
    Mq_tarea mq_tarea_send;/*Estrcutura en el que padre envia las tareas*/
    Mq_tarea mq_tarea_recv;/*Estrcutura en el que hijos recive las tareas*/


    /* The data is loaded and the structure initialized en mem. compartida */
    if (init_sort(file_name, sort, n_levels, n_processes, delay) == ERROR) {
        fprintf(stderr, "sort_multiple_process - init_sort\n");
        return ERROR;
    }

    plot_vector(sort->data, sort->n_elements);
    printf("\nStarting algorithm with %d levels and %d processes...\n", sort->n_levels, sort->n_processes);
    /* For each level, and each part, the corresponding task is solved. */

    /*Paso 4:*/
    for (i = 0; i < sort->n_levels; i++) {
        for (j = 0; j < get_number_parts(i, sort->n_levels); j++) {

            pid = fork();

            if(pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }else if(pid == 0) {

                /*----TRABAJADORES----*/
                /*Cola*/
                mqd_t queue_workers = mq_open(MQ_NAME,
                                      O_CREAT | O_RDONLY, /* This process is only going to send messages */
                                      S_IRUSR | S_IWUSR, /* The user can read and write */
                                      &attributes);
                if(queue_workers == (mqd_t)-1) {
                    fprintf(stderr, "Error opening the queue\n");
                    exit(EXIT_FAILURE);
                }
                if(mq_receive(queue_workers, (char *)&mq_tarea_recv, sizeof(int), NULL) == -1){
                    perror("mq_receive");
                    exit(EXIT_FAILURE);
                }

                solve_task(sort, i, mq_tarea_recv.parte);
                exit(EXIT_SUCCESS);

            }

            /*FIN FOR DE PARTES*/
        }

        /*-----PADRE-----*/
        /*Mete en cola las tareas de este nivel y enviar*/
        for (j = 0; j < get_number_parts(i, sort->n_levels); j++) {
            mq_tarea_send.parte=j;

            if( mq_send(queue, (char*)&mq_tarea_send, sizeof(int), 1) == -1){
                perror("mq_send-padre tareas");
                return ERROR;/*Y liberar todos los recursos?¿?¿?*/
            }
        }
        while(wait(NULL) > 0){}

        /*-----PADRE-----*/
        plot_vector(sort->data, sort->n_elements);
        printf("\n%10s%10s%10s%10s%10s\n", "PID", "LEVEL", "PART", "INI", \
                "END");
        printf("%10d%10d%10d%10d%10d\n", getpid(), i, j, \
                sort->tasks[i][j].ini, sort->tasks[i][j].end);

        /*FIN FOR DE NIVELES*/
    }


    plot_vector(sort->data, sort->n_elements);
    printf("\nAlgorithm completed\n");

    /* Free the shared memory */
    munmap(sort, sizeof(*sort));
    shm_unlink(SHM_NAME);

    /*Free cola*/
    mq_close(queue);
    mq_unlink(MQ_NAME);

    return OK;
}

