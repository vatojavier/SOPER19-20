/**
 * @file ejercicio_prottemp.c
 *
 * Ejercicio 8 en el que se crean procesos y se mandan señales entre ellos
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/3/2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

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
	struct sigaction act_padre, act_hijos;

	if(argc < 3){
		printf("Introduce 2 argumentos\n");
		exit(EXIT_FAILURE);
	}

	n_procesos = atoi(argv[1]);
	segundos = atoi(argv[2]);

	pids = (pid_t*)malloc(sizeof(pid_t)*n_procesos);

	for (int i = 0; i < n_procesos; i++) {
		pids[i] = fork();

		if(pids[i] < 0){
			perror("Fork:");
			exit(EXIT_FAILURE);
		}else if(pids[i] == 0){
			act_hijos.sa_handler = manejador_SIGTERM;
			sigemptyset(&(act_hijos.sa_mask));
			act_hijos.sa_flags = 0;

			if (sigaction(SIGTERM, &act_hijos, NULL) < 0) {
				perror("sigaction");
				exit(EXIT_FAILURE);
			}
			/*Hacer operacion*/

			/*señal a padre*/
			while(1){
				printf("Hijo esperando %d\n",getpid());
				sleep(9999);
			}

			/*No deberían llegar a este exit*/
			exit(EXIT_SUCCESS);
		}

	}

	/*--- PADRE ---*/
	act_padre.sa_handler = manejador_SIGALRM;
	sigemptyset(&(act_padre.sa_mask));
	act_padre.sa_flags = 0;

	/*Se pone alarma*/
	if (alarm(segundos)) {
		fprintf(stderr, "Existe una alarma previa establecida\n");
	}

	/*Se arma manejador de alarma*/
	if (sigaction(SIGALRM, &act_padre, NULL) < 0) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	/*Se arma manejador de SIGTERM para el padre
	 para que no se mate a si mismo*/
	if (sigaction(SIGTERM, &act_padre, NULL) < 0) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	while(1){
		if(got_signal_alrm){
			printf("Matabdo hijod\n");
			for(int i = 0; i < n_procesos; i++){
				kill(pids[i], SIGTERM);
			}
			//kill(0, SIGTERM);
			break;
		}
		printf("Padre esperando\n");
		sleep(9999);
	}

	printf("Finalizado padre\n");


}


