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
        .mq_msgsize = sizeof(Mq_tarea) //* MAX_LONG //2kB
};

/* Variables compartidas*/
static mqd_t queue;
static pid_t *pids;


/*--- MANEJADORES ---*/
void manejador_sigterm(int sig) {
    liberar_recursos(sort, queue);
    exit(EXIT_SUCCESS);
}

void manejador_sigusr1(int sig) {}

void manejador_sigint(int sig){
    /*Enviar señal SIGTERM a los trabajadores*/
    if(senal_todos_hijos(sort->n_processes, pids, SIGTERM) == -1){
        fprintf(stderr, "Error matando a los hijos\n");
    }

    while(wait(NULL) > 0){} 
    liberar_recursos(sort, queue);
    exit(EXIT_FAILURE);
}

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

void liberar_recursos(Sort *sort, mqd_t queue){
    /* Free the shared memory */
    munmap(sort, sizeof(*sort));
    shm_unlink(SHM_NAME);

    /*Free cola*/
    mq_close(queue);
    mq_unlink(MQ_NAME);

}

int armar_manejador(struct sigaction* act, int signal, void (*fun_ptr)(int)){

    if (signal < 0 || fun_ptr == NULL){
        return -1;
    }

    act->sa_handler = fun_ptr;
    sigemptyset(&(act->sa_mask));
    act->sa_flags = 0;
    if (sigaction(signal, act, NULL) < 0) {
        perror("Armar senial:");
        return -1;
    }

    return 0;
}


int senal_todos_hijos(int n_hijos,pid_t *pids, int senial){

    for(int i = 0; i < n_hijos; i++){
        if (kill(pids[i], senial) == -1) {
            perror("kill");
            liberar_recursos(sort, queue);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

/*--- FUNCIONES TOCHAS ---*/

void trabajador(pid_t ppid){

    Mq_tarea mq_tarea_recv;/*Estrcutura en el que hijos recive las tareas*/

    /*Cola*/
    mqd_t queue_workers = mq_open(MQ_NAME,
                          O_CREAT | O_RDONLY, /* This process is only going to send messages */
                          S_IRUSR | S_IWUSR, /* The user can read and write */
                          &attributes);
    if(queue_workers == (mqd_t)-1) {
        fprintf(stderr, "Error opening the queue\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        if(mq_receive(queue_workers, (char *)&mq_tarea_recv, sizeof(Mq_tarea), NULL) == -1){
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }
        printf("Trabajdor %d ha recibido %d, %d\n",getpid(), mq_tarea_recv.nivel, mq_tarea_recv.tarea);

        solve_task(sort, mq_tarea_recv.nivel, mq_tarea_recv.tarea);

        sem_wait(&sort->sem);
        sort->tasks[mq_tarea_recv.nivel][mq_tarea_recv.tarea].completed = COMPLETED;
        sem_post(&sort->sem);

        if (kill(ppid, SIGUSR1) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    }

}

/*#######___ FUNCION PRINCIPAL ___#######*/
Status sort_multiple_process(char *file_name, int n_levels, int n_processes, int delay){
    int i=0, j=0;
    pid_t ppid = getpid();
    struct sigaction act_term, act_usr1, act_int, ign_int;
    Bool completed;

    pids = (pid_t*)malloc(sizeof(pid_t)*n_processes);

    /*Inicializar manejador SIGUSR1 de padre*/
    if(armar_manejador(&act_usr1, SIGUSR1, &manejador_sigusr1) == -1){
        fprintf(stderr, "Error creando manejador sigusr1\n");
        return ERROR;
    }
    /*Inicializar manejador SIGINT*/
    if(armar_manejador(&act_int, SIGINT, &manejador_sigint) == -1){
        fprintf(stderr, "Error creando manejador sigint\n");
        return ERROR;
    } 

    /*Memoria compartida*/
    if(preparar_mem_comp() == ERROR){
        fprintf(stderr, "Error creando mem comp\n");
        return ERROR;
    }

    if(sem_init(&sort->sem, 1, 1) == -1){
        perror("sem_init");
        return ERROR;
    }

    /*Creando colita*/
    queue = mq_open(MQ_NAME,
                    O_CREAT | O_WRONLY, /* This process is only going to send messages */
                    S_IRUSR | S_IWUSR, /* The user can read and write */
                    &attributes);

    if (queue == (mqd_t)-1) {
        perror("mq_open-padre");
        fprintf(stderr, "Error opening the queue - proceso padre - borrar de /dev/mqueue\n");
        return ERROR;
    }
    Mq_tarea mq_tarea_send;/*Estrcutura en el que padre envia las tareas*/

    /* The data is loaded and the structure initialized en mem. compartida */
    if (init_sort(file_name, sort, n_levels, n_processes, delay) == ERROR) {
        fprintf(stderr, "sort_multiple_process - init_sort\n");
        return ERROR;
    }

    plot_vector(sort->data, sort->n_elements);
    printf("\nStarting algorithm with %d levels and %d processes...\n", sort->n_levels, sort->n_processes);
    /* For each level, and each part, the corresponding task is solved. */

    /*Inicializar trabajadores*/
    for (i = 0; i < sort->n_processes; ++i){
        pids[i] = fork();

        if(pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);

        }else if(pids[i] == 0) {

            /*Inicializar manejador SIGTERM de hijos*/
            if(armar_manejador(&act_term, SIGTERM, &manejador_sigterm) == -1){
                fprintf(stderr, "Error creando manejador sigterm\n");
                return ERROR;
            }
            if(armar_manejador(&ign_int, SIGINT, SIG_IGN) == -1){
                fprintf(stderr, "Error creando manejador sig_ign\n");
                return ERROR;
            }

            printf("Trabajador creado\n");
            trabajador(ppid);

        }
    }

    /*Paso 4:*/
    for (i = 0; i < sort->n_levels; i++) {
        mq_tarea_send.nivel = i;

        /*-----PADRE-----*/
        /*Mete en cola las tareas de este nivel y enviar*/
        for (j = 0; j < get_number_parts(i, sort->n_levels); j++) {
            mq_tarea_send.tarea=j;

            printf("Padre enviando\n");
            if(mq_send(queue, (char*)&mq_tarea_send, sizeof(Mq_tarea), 1) == -1){
                perror("mq_send-padre tareas");
                return ERROR;/*Y liberar todos los recursos?¿?¿?*/
            }
        }

        while(wait(NULL) > 0){}//hhhmhmh

        completed = FALSE;
        while(completed == FALSE){

            sem_wait(&sort->sem);

            /*Recorremos las partes para saber cuantas estan completadas*/
            for (j = 0; j < get_number_parts(i, sort->n_levels); ++j){
                completed = TRUE;
                if (sort->tasks[i][j].completed != COMPLETED) {
                    completed = FALSE;
                    /*Si todas las tareas estan completadas, se rompe el bucle y se sigue con el siguiente nivel*/
                }
            }
            sem_post(&sort->sem);
        }

        /*-----PADRE-----*/
        plot_vector(sort->data, sort->n_elements);
        printf("\n%10s%10s%10s%10s%10s\n", "PID", "LEVEL", "PART", "INI", \
                "END");
        printf("%10d%10d%10d%10d%10d\n", getpid(), i, j, \
                sort->tasks[i][j].ini, sort->tasks[i][j].end);

        /*FIN FOR DE NIVELES*/
    }


    /*Enviar señal SIGTERM a los trabajadores*/
    if(senal_todos_hijos(n_processes, pids, SIGTERM) == -1){
        fprintf(stderr, "Error matando a los hijos\n");
    }

    while(wait(NULL) > 0){}


    plot_vector(sort->data, sort->n_elements);
    printf("\nAlgorithm completed\n");

    liberar_recursos(sort, queue);

    return OK;
}