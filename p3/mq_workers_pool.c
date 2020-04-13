/**
 * @file mq_workers_pool.c
 *
 * Programa que instancia los procesos trabajadores del ejercicio 6
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 11/4/2020
 *
 */

#include "pool_trabajadores.h"

#define MAX_HIJOS 10

static volatile sig_atomic_t got_signal_USR2 = 0;
static volatile sig_atomic_t got_signal_TERM = 0;

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

#define MAX_LONG 2000

typedef struct {
    char trozo[MAX_LONG];
} Mensaje;

/*./workers_pool 5 /nombre_cola c */
int main(int argc, char **argv){

    int n_trabajdores;
    int contador, msgs_procesados;
    char caracter_cont;
    pid_t hijos[MAX_HIJOS];
    struct sigaction act_padre, act_hijo;

    struct mq_attr attributes = {
            .mq_flags = 0,
            .mq_maxmsg = 10,
            .mq_curmsgs = 0,
            .mq_msgsize = sizeof(char) * MAX_LONG //2kB??
    };

    if(argc != 4){
        printf("Introduce n trabajadores, nombre de cola y caracter a contar\n");
        exit(EXIT_FAILURE);
    }

    if(argv[2][0] != '/'){
        printf("La cola debe empezar por /\n");
        exit(EXIT_SUCCESS);
    }

    n_trabajdores = (int) strtol(argv[1], NULL, 10);
    caracter_cont = argv[3][0];

    if(n_trabajdores < 1 || n_trabajdores > 10){
        printf("Trabajadores debe ser > 0 y < 10\n");
        exit(EXIT_SUCCESS);
    }

    if(armar_manejador(&act_padre, SIGUSR2, &manejador_USR2) == -1){
        exit(EXIT_FAILURE);
    }

    mqd_t queue = mq_open(argv[2],
                          O_CREAT | O_RDONLY, /* This process is only going to send messages */
                          S_IRUSR | S_IWUSR, /* The user can read and write */
                          &attributes);
    if(queue == (mqd_t)-1) {
        fprintf(stderr, "Error opening the queue\n");
        return EXIT_FAILURE;
    }

    for(int i = 0; i < n_trabajdores; i++){

        hijos[i] = fork();

        if(hijos[i] < 0){
            perror("Fork:");
            exit(EXIT_FAILURE);
        }else if(hijos[i] == 0){

            if(armar_manejador(&act_hijo, SIGTERM, manejador_SIGTERM) == -1){
                exit(EXIT_FAILURE);
            }

            Mensaje msg;
            msgs_procesados = 0;
            contador = 0;
            while(1){

                if (mq_receive(queue, (char *)&msg, sizeof(msg), NULL) == -1) {
                    fprintf(stderr, "Error receiving message\n");
                    return EXIT_FAILURE;
                }

                if(strcmp(msg.trozo, "fin_de_mensaje") == 0){
                    kill(getppid(), SIGUSR2);
                    break;
                }else{
                    contador = contar_caracter(msg.trozo, caracter_cont);
                }
            }

            while(!got_signal_TERM){
                sleep(999);
            }

            printf("Hijo %d:\n \tprocesados: %d\n"
                   " \tcaracteres contados: %d\n", getpid(), msgs_procesados, contador);

            exit(EXIT_SUCCESS);

        }

    }

    /*PADRE*/
    while(!got_signal_USR2){
        sleep(999);
    }

    senal_todos_hijos(n_trabajdores,hijos, SIGTERM);

    mq_close(queue);
    exit(EXIT_SUCCESS);
}