/**
 * @file ejercicio_lect_escr.c
 *
 * Ejercicio 14 en el que se simula problema escritores lectores
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 14/3/2020
 *
 */

#include "prottemp_mejorado.h"

#define SEM_NAME_ESCR "/sem_escritores"
#define SEM_NAME_LECT "/sem_lectores"
#define SEM_NAME_CONT_LECT "/sem_cont_lectores"
#define N_READ 10
#define SECS 1

static volatile sig_atomic_t got_signal_INT = 0;
static volatile sig_atomic_t got_signal_TERM = 0;

/***************************************************************
Nombre: manejador_SIGTERM.
Descripcion:
    manejador de la señal de SIGTERM que viene del padre
Entrada:
    int sig: señal
Salida:
************************************************************/
void manejador_SIGTERM(int sig) {
    got_signal_TERM ++;
}

/***************************************************************
Nombre: manejador_SIGINT.
Descripcion:
    manejador de la señal de SIGINT
Entrada:
    int sig: señal
Salida:
************************************************************/
void manejador_SIGINT(int sig) {
    got_signal_INT ++;
}

int main(int argc, char **argv){
    struct sigaction act_padre, act_hijo;
    pid_t pids[N_READ];
    sem_t *sem_escritores = NULL;
    sem_t *sem_lectores = NULL;
    sem_t *sem_cont_lectores = NULL;

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
        perror("sem_open cont_lectores");
        exit(EXIT_FAILURE);
    }

    /*Manejador del padre*/
    if(armar_manejador(&act_padre, SIGINT, &manejador_SIGINT) == -1){
        printf("Error armando manejador\n");
        exit(EXIT_FAILURE);
    }

    /*Manejador del hijo*/
    if(armar_manejador(&act_hijo, SIGTERM, &manejador_SIGTERM) == -1){
        printf("Error armando manejador\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < N_READ; i++){

        pids[i] = fork();

        if(pids[i] < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0){

            while(!got_signal_TERM){

                /*--- HIJOS lectores ---*/
                while(sem_wait(sem_lectores) == -1 && errno == EINTR)
                    continue;
                sem_post(sem_cont_lectores);
                if(get_valor_semaforo(sem_cont_lectores, SEM_NAME_CONT_LECT) == 1){
                    sem_wait(sem_escritores);
                }
                sem_post(sem_lectores);

                printf("R-INI %d\n", getpid());
                sleep(1);
                printf("R FIN %d\n", getpid());

                while(sem_wait(sem_lectores) == -1 && errno == EINTR)
                    continue;
                sem_wait(sem_cont_lectores);
                if(get_valor_semaforo(sem_cont_lectores, SEM_NAME_CONT_LECT) == 0){
                    sem_post(sem_escritores);
                }
                sem_post(sem_lectores);

                sleep(SECS);
            }

            sem_close(sem_cont_lectores);
            sem_close(sem_lectores);
            sem_close(sem_escritores);

            exit(EXIT_SUCCESS);

        }
    }

    /*--- PADRE escritor---*/
    while(!got_signal_INT){

        while(sem_wait(sem_escritores) == -1 && errno == EINTR)
            continue;

        printf("W-INI %d\n", getpid());
        sleep(1);
        printf("W-FIN %d\n", getpid());

        sem_post(sem_escritores);

        sleep(SECS);
    }

    if(senal_todos_hijos(N_READ, pids, SIGTERM) == -1){
        printf("concha\n");
        exit(EXIT_FAILURE);
    }


    sem_unlink(SEM_NAME_CONT_LECT);
    sem_unlink(SEM_NAME_LECT);
    sem_unlink(SEM_NAME_ESCR);

    sem_close(sem_cont_lectores);
    sem_close(sem_lectores);
    sem_close(sem_escritores);

    exit(EXIT_SUCCESS);

}