#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
	int fd[2];

	const char *string = "Hola a todos!\n";
	char readbuffer[80];

	int pipe_status = pipe(fd);
	if(pipe_status == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t childpid = fork();
	if(childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (childpid == 0) {

		/* Cierre del descriptor de entrada,mal: salida en el hijo */
		close(fd[0]);

		/* Enviar el saludo vía descriptor de salida */
		/* strlen(string) + 1 < PIPE_BUF así que no hay escrituras cortas */
		ssize_t nbytes = write(fd[1], string, strlen(string) + 1);
		if(nbytes == -1)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}

		printf("He escrito en el pipe\n");

		exit(EXIT_SUCCESS);
	} else {
		/* Cierre del descriptor de salida mal: entrada (escritura) en el padre */
		/*close(fd[1]);*/
		/* Leer algo de la tubería... el saludo! */
		ssize_t nbytes = 0;
		do {
			nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
			if(nbytes == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
			if(nbytes > 0)
			{
				printf("He recibido el string: %.*s", (int) nbytes, readbuffer);
			}
		} while(nbytes != 0);

		wait(NULL);
		exit(EXIT_SUCCESS);
	}

	/*Para ej 15 cerrar las tuberias no utilizadas en cada proceso, ej1 cierra tuberia2 completamente*/
}

