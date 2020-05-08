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
        .mq_msgsize = sizeof(Mq_tarea)
};

/* Variables compartidas*/
static mqd_t queue;
static pid_t *pids;

/*Tuberias*/
int pipe_in_ilustrador[512][2]; //tuberias ilustrador recibe tarea de trabajadores
int pipe_out_ilustrador[512][2]; //tuberias ilustrador envia a trabajadores que ya se ha impreso la movida
int pipe_prueba[2];
static volatile sig_atomic_t got_signal_alrm = 0;




/*--- MANEJADORES ---*/
void manejador_sigterm(int sig) {
    liberar_recursos();
    exit(EXIT_SUCCESS);
}

void manejador_sigusr1(int sig) {}

void manejador_sigint(int sig){
    /*Enviar señal SIGTERM a los trabajadores*/

    printf("Liberando\n");

    if(senal_todos_hijos(sort->n_processes, SIGTERM) == -1){
        fprintf(stderr, "Error matando a los hijos\n");
    }

    while(wait(NULL) > 0){} 
    liberar_recursos();
    exit(EXIT_FAILURE);
}

void manejador_alarm(int sig){

    got_signal_alrm ++;
}

void manejador_sigterm_ilu(int sig){
    exit(EXIT_SUCCESS);
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

void liberar_recursos(){
    /* Free the shared memory */
    munmap(sort, sizeof(*sort));
    shm_unlink(SHM_NAME);

    /*Free cola*/
    mq_close(queue);
    mq_unlink(MQ_NAME);

    free(pids);
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


int senal_todos_hijos(int n_hijos, int senial){

    for(int i = 0; i < n_hijos; i++){
        if (kill(pids[i], senial) == -1) {
            perror("kill");
            liberar_recursos();
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

Bool tarea_libre(){

    for(int i = 0; i < get_number_parts(sort->nivel_trabajo, sort->n_levels); i++){
        if(sort->tasks[sort->nivel_trabajo][i].completed == INCOMPLETE){
            return TRUE;//Hay tasks a realizar
        }
    }

    return FALSE;
}

/*--- FUNCIONES PARA TUBERIAS ---*/
Status read_stat_de(int *pipe, int *pid, int *nivel, int *parte){
    int n_leidos = 0;
    int tarea[3];

    *nivel=0;

    ssize_t nbytes = 0;
    do {
        nbytes = read(pipe[0], &tarea[n_leidos], sizeof(int));
        if(nbytes == -1)
        {
            printf("Error leyendo\n");
            perror("read stat de");
            parte[0]=-1;
            return ERROR;
        }
        n_leidos++;
    } while(nbytes != 0 && n_leidos < 3);

    *pid = tarea[0];
    *nivel = tarea[1];
    *parte = tarea[2];

    return OK;
}

Status write_stat_en(int *pipe, int nivel, int parte){
    int tarea[3];

    tarea[0] = getpid();
    tarea[1] = nivel;
    tarea[2] = parte;

    ssize_t nbytes = write(pipe[1], tarea, sizeof(int)*3);
    if(nbytes == -1)
    {
        perror("write stat en");
        return ERROR;
    }
    return OK;
}

Status write_cont(int *pipe){
    int cont = 1;

    ssize_t nbytes = write(pipe[1], &cont, sizeof(int));
    if(nbytes == -1)
    {
        perror("write cont");
        return ERROR;
    }
    return OK;

}

Status read_cont(int *pipe){
    int n_leidos = 0;
    int cont;

    /* Leer algo de la tubería*/
    ssize_t nbytes = 0;
    do {

        do{
            nbytes = read(pipe[0], &cont, sizeof(int));
        }while(nbytes == -1 && errno == EINTR);

        if(nbytes == -1)
        {
            printf("Error leyendo\n");
            perror("read cont");
            return ERROR;
        }
        n_leidos++;
    } while(nbytes != 0 && n_leidos < 1);

    return OK;
}

Status crear_tuberias(){
    int i;

    for(i = 0; i < sort->n_processes; i++){
        if(pipe(pipe_in_ilustrador[i]) == -1){
            perror("pipe_in");
            return ERROR;
        }

        if(pipe(pipe_out_ilustrador[i]) == -1){
            printf("Error creando tuberias out_ilustrador\n");
            return ERROR;
        }
    }

    return OK;

}

/*--- FUNCIONES TOCHAS ---*/
void trabajador(pid_t ppid, int tuberia){

    Mq_tarea mq_tarea_recv;/*Estrcutura en el que hijos recibe las tareas*/
    int ret;

    /*Cola*/
    mqd_t queue_workers = mq_open(MQ_NAME,
                          O_CREAT | O_RDONLY, /* This process is only going to send messages */
                          S_IRUSR | S_IWUSR, /* The user can read and write */
                          &attributes);
    if(queue_workers == (mqd_t)-1) {
        fprintf(stderr, "Error opening the queue\n");
        exit(EXIT_FAILURE);
    }

    if (alarm(SECS)) {
        fprintf(stderr, "Existe una alarma previa establecida\n");
    }

    //Preparando tuberias
    /*La de enviar*/
    if(close(pipe_in_ilustrador[tuberia][0]) == -1){
        printf("Error cerrando wse\n");
    }
    /*La de recibir cont*/
    if(close(pipe_out_ilustrador[tuberia][1]) == -1){
        printf("Error cerrando wse\n");
    }

    while(1){

        //Ver si todas las tareas del nivel actual se están procesando/completadas, si es asi, no esperar mensaje de la cola

        do{
            ret = mq_receive(queue_workers, (char *)&mq_tarea_recv, sizeof(Mq_tarea), NULL);

            if(ret == -1 && errno == EINTR && got_signal_alrm){
                write_stat_en(pipe_in_ilustrador[tuberia], 11, 11);
                read_cont(pipe_out_ilustrador[tuberia]);
                got_signal_alrm = 0;
                alarm(SECS);
            }

        }while(ret == -1 && errno == EINTR);

        solve_task(sort, mq_tarea_recv.nivel, mq_tarea_recv.tarea);

        while(sem_wait(&sort->sem) == -1 && errno == EINTR);
        sort->tasks[mq_tarea_recv.nivel][mq_tarea_recv.tarea].completed = COMPLETED;
        while(sem_post(&sort->sem) == -1 && errno == EINTR);

        if (kill(ppid, SIGUSR1) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }


        if(got_signal_alrm){
            got_signal_alrm = 0;
            alarm(SECS);

            write_stat_en(pipe_in_ilustrador[tuberia], mq_tarea_recv.nivel, mq_tarea_recv.tarea);

            read_cont(pipe_out_ilustrador[tuberia]);

        }
    }

}

Status ilustrador(){
    struct sigaction act_term_il;
    int nivel, tarea, pid;
    int pid_worker[512];
    int nivel_worker[512];
    int tarea_worker[512];

    /*Inicializar manejador term de ilustrador*/
    if(armar_manejador(&act_term_il, SIGTERM, &manejador_sigterm_ilu) == -1){
        fprintf(stderr, "Error creando manejador sigusr1\n");
        return ERROR;
    }

    //Preparar tuberias
    /*Las de leer y enviar cont*/
    for(int i = 0; i < sort->n_processes; i++){
        if(close(pipe_in_ilustrador[i][1]) == -1){
            printf("Error cerrando desc\n");
        }
        if(close(pipe_out_ilustrador[i][0]) == -1){
            printf("Error cerrando desc\n");
        }
    }

    while(1){
        for(int i = 0; i < sort->n_processes; i++){
            read_stat_de(pipe_in_ilustrador[i], &pid_worker[i], &nivel_worker[i], &tarea_worker[i]);
        }
        plot_vector(sort->data, sort->n_elements);

        /*Imprimiendo estado*/
        printf("\n%10s%10s%10s%10s%10s\n", "PID", "LEVEL", "PART", "INI", \
                "END");
        for(int i = 0; i < sort->n_processes; i++){
            pid = pid_worker[i];
            nivel = nivel_worker[i];
            tarea = tarea_worker[i];

                printf("%10d%10d%10d%10d%10d\n", pid, nivel, tarea,
                        sort->tasks[nivel][tarea].ini, sort->tasks[nivel][tarea].end);
        }

        /*Si la (unica) tarea del ultimo nivel está impresa y completada...*/
        if(sort->tasks[sort->n_levels - 1][0].completed == COMPLETED){
            sem_post(&sort->sem_fin);
        }

        /*Enviar que pueden continuar*/
        for(int i=0; i < sort->n_processes; i++){
            write_cont(pipe_out_ilustrador[i]);
        }
    }
}

/*#######___ FUNCION PRINCIPAL ___#######*/
Status sort_multiple_process(char *file_name, int n_levels, int n_processes, int delay){
    int i=0, j=0, ret=0;
    int pid;
    pid_t ppid = getpid();
    struct sigaction act_term, act_usr1, act_int, ign_int, act_alrm;
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

    if(sem_init(&sort->sem_fin, 1, 0) == -1){
        perror("sem_init fin");
        return ERROR;
    }

    if(sem_init(&sort->sem_nivel_trabajo, 1, 1) == -1){
        perror("sem_init nivel");
        return ERROR;
    }

    if(sem_init(&sort->sem_pipe, 1, 1) == -1){
        perror("sem_init nivel");
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

    printf("\nStarting algorithm with %d levels and %d processes...\n", sort->n_levels, sort->n_processes);
    /* For each level, and each part, the corresponding task is solved. */

    crear_tuberias();
    pipe(pipe_prueba);

    /*Ilustrador*/
    pid = fork();
    if(pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0){
        ilustrador();
        exit(EXIT_SUCCESS);
    }

    /*Inicializar trabajadores*/
    for (i = 0; i < sort->n_processes; ++i){
        pids[i] = fork();

        if(pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);

        }else if(pids[i] == 0) {

            /*Inicializar manejadores de hijos*/
            if(armar_manejador(&act_term, SIGTERM, &manejador_sigterm) == -1){
                fprintf(stderr, "Error creando manejador sigterm\n");
                return ERROR;
            }
            if(armar_manejador(&ign_int, SIGINT, SIG_IGN) == -1){
                fprintf(stderr, "Error creando manejador sig_ign\n");
                return ERROR;
            }

            if(armar_manejador(&act_alrm,SIGALRM,&manejador_alarm) == -1){
                fprintf(stderr, "Error creando manejador sig_alarm\n");
                return ERROR;
            }
            trabajador(ppid, i);
        }
    }

    /*-----PADRE-----*/
    for (i = 0; i < sort->n_levels; i++) {
        mq_tarea_send.nivel = i;

        while(sem_wait(&sort->sem_nivel_trabajo) == -1 && errno ==EINTR);
        sort->nivel_trabajo = i;
        while(sem_post(&sort->sem_nivel_trabajo) == -1 && errno ==EINTR);

        /*Mete en cola las tareas de este nivel y enviar*/
        for (j = 0; j < get_number_parts(i, sort->n_levels); j++) {
            mq_tarea_send.tarea=j;

            do{
                ret = mq_send(queue, (char*)&mq_tarea_send, sizeof(Mq_tarea), 1);
            }while(ret == -1 && errno == EINTR);//Si es interrumpido por usr se itenta otra vez
            if(ret == -1 && errno != EINTR){
                perror("mq_send-padre tareas");
                return ERROR;/*Y liberar todos los recursos?¿?¿?*/
            }
        }

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

        /*FIN FOR DE NIVELES*/
    }

    /*Hasta que ilustrador no imprima último estado no mata a nadie*/
    while(sem_wait(&sort->sem_fin) == -1 && errno == EINTR);

    /*Enviar señal SIGTERM a los trabajadores CUANDO TERMINE DE IMPRIMIR NO?*/
    if(senal_todos_hijos(n_processes, SIGTERM) == -1){
        fprintf(stderr, "Error matando a los hijos\n");
    }

    if (kill(pid, SIGTERM) == -1) {
        perror("kill");
        liberar_recursos();
        exit(EXIT_FAILURE);
    }

    while(wait(NULL) > 0){}
    printf("\nAlgorithm completed\n");

    liberar_recursos();

    return OK;
}