/**
 * @file mq_injector.c
 *
 * Programa que envia contenido del ejercicio 6
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 11/4/2020
 *
 */

#include "pool_trabajadores.h"

#define MAX_LONG 2000

typedef struct {
    char trozo[MAX_LONG];
} Mensaje;

/*./mq_injector fichero_entrada.txt / nombre_cola*/
int main(int argc, char **argv){

    FILE *fp;
    char buffer[MAX_LONG];

    struct mq_attr attributes = {
            .mq_flags = 0,
            .mq_maxmsg = 10,
            .mq_curmsgs = 0,
            .mq_msgsize = sizeof(char) * MAX_LONG //2kB??
    };

    if(argc != 3){
        printf("Introduce fichero y nombre de la cola\n");
        exit(EXIT_FAILURE);
    }

    if(argv[2][0] != '/'){
        printf("La cola debe empezar por /\n");
        exit(EXIT_SUCCESS);
    }

    /*Abriendo fichero*/
    fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("Archivo no existe\n");
        exit(EXIT_FAILURE);
    }

    /*Creando colita*/
    mqd_t queue = mq_open(argv[2],
                          O_CREAT | O_WRONLY, /* This process is only going to send messages */
                          S_IRUSR | S_IWUSR, /* The user can read and write */
                          &attributes);

    if (queue == (mqd_t)-1) {
        fprintf(stderr, "Error opening the queue\n");
        return EXIT_FAILURE;
    }

    Mensaje msg;
    while(!feof(fp)){
        fread(buffer, sizeof(buffer) , 1, fp); //Enviando buffer de 2kB

        //Enviar mensaje a la cola
        strcpy(msg.trozo, buffer);
        int ret = mq_send(queue, (char*)&msg, sizeof(msg), 1);
        if(ret == -1){
            perror("mq_send");
            break;
        }
    }

    /*Enviando mensaje de finalizacion*/
    strcpy(msg.trozo, "fin_de_mensaje");
    int ret = mq_send(queue, (char*)&msg, sizeof(msg), 1);
    if(ret == -1){
        perror("mq_send");
    }

    mq_close(queue);
    mq_unlink(argv[2]);
    fclose(fp);
    exit(EXIT_SUCCESS);
}