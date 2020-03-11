/**
 * @file ejercicio_prottemp.c
 *
 * Ejercicio 8 en el que se crean procesos y se mandan señales entre ellos
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/3/2020
 * TODO: comprobar sumatorio
 */
#include "prottemp.h"

static volatile sig_atomic_t got_signal_alrm = 0;
static volatile sig_atomic_t got_signal_USR2 = 0;

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

int main(int argc, char **argv) {

	int n_procesos, segundos;
	pid_t *pids;
	struct sigaction act_padre,act_padre_usr, act_hijos;

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

	for (int i = 0; i < n_procesos; i++) {
		pids[i] = fork();

		if(pids[i] < 0){
			perror("Fork:");
			exit(EXIT_FAILURE);

		}else if(pids[i] == 0){
			long res;

			if(armar_manejador(&act_hijos, SIGTERM, &manejador_SIGTERM) == -1){
				exit(EXIT_FAILURE);
			}

			res = sumar_numeros();
			printf("Hijo %d, resultado=%ld\n", getpid(), res);

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
		if(got_signal_alrm){
			if(senal_todos_hijos(n_procesos, pids, SIGTERM) == -1){
				exit(EXIT_FAILURE);
			}
			break;
		}
		sleep(9999);
	}

	printf("Finalizado padre, señales SIGUSR2 recibidas: %d\n",got_signal_USR2);
	while(wait(NULL) > 0){}

	exit(EXIT_SUCCESS);
}


