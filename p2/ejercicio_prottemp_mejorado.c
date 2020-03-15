/**
 * @file ejercicio_prottemp_mejorado.c
 *
 * Ejercicio 12 en el que se crean procesos y se mandan señales entre ellos con concurrencia
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 9/4/2020
 *
 * todo: fugas mem, hacer sem_close en hijos
 * BORRA SEMAFOROS EN /dev/smh/<nombresem>
 * NO HACERLO CON DO WHILE!
 */

#include "prottemp_mejorado.h"

static volatile sig_atomic_t got_signal_alrm = 0;
static volatile sig_atomic_t got_signal_USR2 = 0;
static volatile sig_atomic_t got_signal_TERM = 0;

#define SEM_NAME_ESCR "/sem_escritores"
#define SEM_NAME_LECT "/sem_lectores"
#define SEM_NAME_CONT_LECT "/sem_cont_lectores"
#define FILE_NAME "data.txt"
#define MAX_PROC 9999

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
    got_signal_TERM++;
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
    int proc_term = 0; /*Procesos terminados*/
    unsigned long res_ant = 0;
    int usr2_ant = 0;
    unsigned long res = 0;
    pid_t pids[MAX_PROC];
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

    if(n_procesos > MAX_PROC){
        printf("Demasiados procesos\n");
        exit(EXIT_FAILURE);
    }

    /*Se arma manejador de USR2 que vienen de los hijos*/
    if(armar_manejador(&act_padre_usr, SIGUSR2, &manejador_USR2) == -1){
        exit(EXIT_FAILURE);
    }

    /*Abrir e inicializar fichero*/
    fp = fopen(FILE_NAME,  "w+");
    fprintf(fp, "0\n0\n");
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
        perror("sem_open cont_lectores");
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
            /*printf("Hijo %d, resultado=%ld\n", getpid(), res);*/

            /*--- SEMAFOROS escribir---*/
            /*Tambien deben leer de 1 en 1!*/
            while((sem_wait(sem_escritores)) == -1 && errno == EINTR)
                continue;

            if(leer_numeros(FILE_NAME, &proc_term, &res_ant) == -1){
                printf("Mal leer numeros hijos %d\n",proc_term );
                /*exit(EXIT_FAILURE);*/
            }

            proc_term ++;
            res_ant += res;

            fp = fopen(FILE_NAME, "w+");
            fprintf(fp, "%d\n%ld\n", proc_term, res_ant);
            fclose(fp);

            sem_post(sem_escritores);
            /*--- FIN SEMAFOROS escribir---*/

            /*señal a padre*/
            kill(getppid(), SIGUSR2);

            while(!got_signal_TERM){
                sleep(9999);
            }

            sem_close(sem_cont_lectores);
            sem_close(sem_lectores);
            sem_close(sem_escritores);

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
            usr2_ant = got_signal_USR2;

            /*--- SEMAFOROS leer---*/
            while((sem_wait(sem_lectores)) == -1 && errno == EINTR)
                continue;

            sem_post(sem_cont_lectores);
            if(get_valor_semaforo(sem_cont_lectores, SEM_NAME_CONT_LECT) == 1){
                while((sem_wait(sem_escritores)) == -1 && errno == EINTR)
                    continue;
            }

            sem_post(sem_lectores);

            /*comprobar fichero*/
            if(leer_numeros(FILE_NAME, &proc_term, &res_ant) == -1){
                printf("\tLEER NUM PADRES MAL %d\n", proc_term);
                exit(EXIT_FAILURE);
            }

            /*Si todos los hijos han terminado se les mata*/
            if(proc_term == n_procesos){
                if(senal_todos_hijos(n_procesos, pids, SIGTERM) == -1){
                    exit(EXIT_FAILURE);
                }
                printf("\tHan acabado todos, resultado: %ld\n", res_ant);
                break;
            }

            while((sem_wait(sem_lectores)) == -1 && errno == EINTR)
                continue;
            sem_wait(sem_cont_lectores);
            if(get_valor_semaforo(sem_cont_lectores, SEM_NAME_CONT_LECT) == 0){
                sem_post(sem_escritores);
            }
            sem_post(sem_lectores);
            /*--- FIN SEMAFOROS leer---*/
        }

        if(got_signal_alrm){

            printf("\tFalta trabajo\n");
            if(senal_todos_hijos(n_procesos, pids, SIGTERM) == -1){
                exit(EXIT_FAILURE);
            }
            break;
        }
        sleep(9999);
    }

    /*printf("Finalizado padre, señales SIGUSR2 recibidas: %d\n",got_signal_USR2);*/
    while(wait(NULL) > 0){}

    printf("\t\nliberando\n");
    sem_unlink(SEM_NAME_CONT_LECT);
    sem_unlink(SEM_NAME_LECT);
    sem_unlink(SEM_NAME_ESCR);

    sem_close(sem_cont_lectores);
    sem_close(sem_lectores);
    sem_close(sem_escritores);

    exit(EXIT_SUCCESS);

}