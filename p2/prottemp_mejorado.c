//
// Created by javi on 9/3/20.
//

#include "prottemp_mejorado.h"


int senal_todos_hijos(int n_hijos,pid_t *pids, int senial){

    for(int i = 0; i < n_hijos; i++){
        kill(pids[i], senial);
    }

    return 0;
}

long sumar_numeros(){
    pid_t pid = getpid();
    long res = 1;

    for(int i = 0; i < (pid/10); i++){
        res = i + res;
    }

    return res;
}


int armar_manejador(struct sigaction* act, int signal, void (*fun_ptr)(int)){

    if (signal < 0 || fun_ptr == NULL){
        return -1;
    }

    act->sa_handler = fun_ptr;
    sigemptyset(&(act->sa_mask));
    act->sa_flags = 0;
    if (sigaction(signal, act, NULL) < 0) {
        perror("Armar senial:");
        return -1;
    }

    return 0;
}

int get_valor_semaforo(sem_t *sem, char* sem_name){
    int sval;

    if (sem_getvalue(sem, &sval) == -1) {
        perror("sem_getvalue");
        sem_unlink(sem_name);
        exit(EXIT_FAILURE);
    }

    return sval;
}
