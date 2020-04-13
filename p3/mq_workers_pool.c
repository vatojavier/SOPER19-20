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
    int ret;
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

    printf("Hijos esperando a leer de la cola\n\n");

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

            while(!got_signal_TERM){

                ret = mq_receive(queue, (char *)&msg, sizeof(msg), NULL);

                if(ret == -1 && errno != EINTR){//si hay fallo
                    perror("mq");
                    fprintf(stderr, "Error receiving message\n");
                    return EXIT_FAILURE;
                }else if(ret == -1){// si es interumpido por llamada no pasa na
                    break;
                }

                if(strcmp(msg.trozo, "fin_de_mensaje") == 0){
                    kill(getppid(), SIGUSR2);
                    break;
                }else{
                    msgs_procesados++;
                    contador += contar_caracter(msg.trozo, caracter_cont);
                }
            }

            printf("Hijo %d-> procesados: %d, caracteres contados: %d.\n", getpid(), msgs_procesados, contador);

            exit(EXIT_SUCCESS);
        }
    }

    /*PADRE*/
    while(!got_signal_USR2){
        sleep(999);
    }

    senal_todos_hijos(n_trabajdores,hijos, SIGTERM);

    while(wait(NULL) > 0){}

    mq_close(queue);
    exit(EXIT_SUCCESS);
}