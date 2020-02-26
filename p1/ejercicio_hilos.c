/**
 * @file ejercicio_hilos.c
 *
 * Ejercicio en el que se crean hilos y esos computan una operación matemática
 * 
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 20/2/2020
 *
 */

#include "hilos.h"
#define MAX_HILOS 2000

int main(int argc, char *argv[]) {
	pthread_t *threadsIds = NULL;

	Args estructuras [MAX_HILOS];/*Array de argumentos para los hilos*/

	int error, nProc;
	int resultado;

	if(argc < 2){
		fprintf(stderr, "Error en argumentos \n");
		exit(EXIT_FAILURE);
	}
	
	nProc = pasarAint(argv[1]);
	if(nProc < 0){
		fprintf(stderr, "Error en argumentos\n");
		exit(EXIT_FAILURE);
	}

	/*Crear memoria para los ids de los threads*/
	threadsIds = (pthread_t*)malloc(sizeof(pthread_t) * nProc);


	srand(time(NULL));

	for(int i=0; i < nProc; i++){
		
		if(setParametros(&estructuras[i], i) == -1){
			printf("Error iniciando parametros de pthread_create");
			exit(EXIT_FAILURE);
		}

		error = pthread_create(&threadsIds[i], NULL, esperarYcalcular, &estructuras[i]);
		if(error != 0)
		{
			fprintf(stderr, "pthread_create: %s\n", strerror(error));
			exit(EXIT_FAILURE);
		}

	}

	/*Esperando a los threads*/
	for(int i=0; i < nProc; i++){
		pthread_join(threadsIds[i], (void**)&resultado);
		printf("Resultado de %ld es %d\n",threadsIds[i], resultado);

	}

	free(threadsIds);
	printf("El programa %s termino correctamente \n", argv[0]);
}
