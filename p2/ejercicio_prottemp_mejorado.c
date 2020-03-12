/**
 * @file ejercicio_prottemp_mejoradp.c
 *
 * Ejercicio 12 en el que se crean procesos y se mandan señales entre ellos con concurrencia
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 9/4/2020
 *
 * BORRA SEMAFOROS EN /dev/smh/<nombresem>
 */

#include "prottemp_mejorado.h"

static volatile sig_atomic_t got_signal_alrm = 0;
static volatile sig_atomic_t got_signal_USR2 = 0;

#define SEM_NAME_ESCR "/sem_escritores"
#define SEM_NAME_LECT "/sem_lectores"
#define SEM_NAME_CONT_LECT "/sem_cont_lectores"
#define FILE_NAME "data.txt"

/***************************************************************
Nombre: manejador_USR2.
Descripcion:
    manejador de la señal que viene de los hijos
Entrada:
    int sig: señal
Salida:
************************************************************/
void manejador_USR2(int sig){
    got_signal_USR2 ++;
}

/***************************************************************
Nombre: manejador_SIGTERM.
Descripcion:
    manejador de la señal de TERM que enviará el padre a los hijos
Entrada:
    int sig: señal
Salida:
************************************************************/
void manejador_SIGTERM(int sig) {
    printf("Finalizado %d\n", getpid());
    exit(EXIT_SUCCESS);
}


/***************************************************************
Nombre: manejador_SIGALRM.
Descripcion:
    manejador de la señal de alarma
Entrada:
    int sig: señal
Salida:
************************************************************/
void manejador_SIGALRM(int sig) {
    got_signal_alrm = 1;
}

int main(int argc, char **argv){

    int n_procesos, segundos;
    int proc_term; /*Procesos terminados*/
    int usr2_ant = 0;
    unsigned long res, res_ant;
    pid_t *pids;
    struct sigaction act_padre,act_padre_usr, act_hijos;
    FILE *fp;
    sem_t *sem_escritores = NULL;
    sem_t *sem_lectores = NULL;
    sem_t *sem_cont_lectores = NULL;


    if(argc < 3){
        printf("Introduce 2 argumentos\n");
        exit(EXIT_FAILURE);
    }

    n_procesos = atoi(argv[1]);
    segundos = atoi(argv[2]);

    pids = (pid_t*)malloc(sizeof(pid_t)*n_procesos);

    /*Se arma manejador de USR2 que vienen de los hijos*/
    if(armar_manejador(&act_padre_usr, SIGUSR2, &manejador_USR2) == -1){
        exit(EXIT_FAILURE);
    }

    /*Abrir fichero e inicializar fichero*/
    fp = fopen(FILE_NAME,  "w+");
    fprintf(fp, "0\n0");
    fclose(fp);

    /*Crear e inicializar semáforos*/
    if ((sem_escritores = sem_open(SEM_NAME_ESCR, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
        perror("sem_open escritores");
        exit(EXIT_FAILURE);
    }

    if ((sem_lectores = sem_open(SEM_NAME_LECT, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
        perror("sem_open lectores");
        exit(EXIT_FAILURE);
    }

    if ((sem_cont_lectores = sem_open(SEM_NAME_CONT_LECT, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        perror("sem_open lectores");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n_procesos; i++) {
        pids[i] = fork();

        if(pids[i] < 0){
            perror("Fork:");
            exit(EXIT_FAILURE);

        }else if(pids[i] == 0){

            if(armar_manejador(&act_hijos, SIGTERM, &manejador_SIGTERM) == -1){
                exit(EXIT_FAILURE);
            }

            res = sumar_numeros();
            printf("Hijo %d, resultado=%ld\n", getpid(), res);

            /*--- SEMAFOROS leer ---*/
            sem_wait(sem_lectores);
            sem_post(sem_cont_lectores);
            if(get_valor_semaforo(sem_cont_lectores, SEM_NAME_CONT_LECT) == 1){
                /*Si soy el primer lector evito que me escriban*/
                sem_wait(sem_escritores);
            }
            sem_post(sem_lectores);

            if(leer_numeros(FILE_NAME, &proc_term, &res_ant) == -1){
                exit(EXIT_FAILURE);
            }
            printf("Los numeros leidos son %d y %ld\n", proc_term, res_ant);

            proc_term ++;
            res_ant += res;

            sem_wait(sem_lectores);
            sem_wait(sem_cont_lectores);
            if(get_valor_semaforo(sem_cont_lectores, SEM_NAME_CONT_LECT) == 0){
                /*Si soy el último ya pueden escribir*/
                sem_post(sem_escritores);
            }
            sem_post(sem_lectores);
            /*--- FIN SEMAFOROS leer---*/

            /*--- SEMAFOROS escribir---*/
            sem_wait(sem_escritores);

            fp = fopen(FILE_NAME, "w+");
            fprintf(fp, "%d\n%ld", proc_term, res_ant);
            fclose(fp);

            sem_post(sem_escritores);
            /*--- FIN SEMAFOROS escribir---*/


            /*señal a padre*/
            kill(getppid(), SIGUSR2);

            while(1){
                sleep(9999);
            }

            /*No deberían llegar a este exit*/
            exit(EXIT_SUCCESS);
        }
    }

    /*--- PADRE ---*/
    /*Se arma manejador de alarma*/
    if(armar_manejador(&act_padre, SIGALRM, &manejador_SIGALRM) == -1){
        exit(EXIT_FAILURE);
    }

    /*Se pone alarma*/
    if (alarm(segundos)) {
        fprintf(stderr, "Existe una alarma previa establecida\n");
    }

    while(1){

        if(usr2_ant != got_signal_USR2){
            /*Si ha llegado nueva señal de usr2*/

            /*--- SEMAFOROS leer---TODO: comprobar que cuando llegan señales esperando semaforo se la come*/
            sem_wait(sem_lectores);
            sem_post(sem_cont_lectores);
            if(get_valor_semaforo(sem_cont_lectores, SEM_NAME_CONT_LECT) == 1){
                sem_wait(sem_escritores);
            }

            sem_post(sem_lectores);

            /*comprobar fichero*/
            if(leer_numeros(FILE_NAME, &proc_term, &res_ant) == -1){
                exit(EXIT_FAILURE);
            }

            /*Si todos los hijos han termanado se les mata*/
            if(proc_term == n_procesos){
                if(senal_todos_hijos(n_procesos, pids, SIGTERM) == -1){
                    exit(EXIT_FAILURE);
                }
                break;
            }

            /*--- SEMAFOROS ---*/
            usr2_ant = got_signal_USR2;
        }
        if(got_signal_alrm){
            //TODO: mirar si han finalizado todos los hijos
            if(senal_todos_hijos(n_procesos, pids, SIGTERM) == -1){
                exit(EXIT_FAILURE);
            }
            break;
        }
        sleep(9999);
    }

    printf("Finalizado padre, señales SIGUSR2 recibidas: %d\n",got_signal_USR2);
    while(wait(NULL) > 0){}

    sem_close(sem_cont_lectores);
    sem_close(sem_lectores);
    sem_close(sem_escritores);

    sem_unlink(SEM_NAME_CONT_LECT);
    sem_unlink(SEM_NAME_LECT);
    sem_unlink(SEM_NAME_ESCR);


    exit(EXIT_SUCCESS);

}