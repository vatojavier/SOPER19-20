#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char *argv[]) {

    int signal, proceso;

    if(argc < 3){
        printf("Introduce dos argumentos\n");
        exit(EXIT_FAILURE);
    }

    signal = atoi(argv[1] + 1);
    proceso = atoi(argv[2]);

    kill(proceso, signal);

}