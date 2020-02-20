#include "hilos.h"

int pasarAint(char *argv){
    char *p;
    long conv;

    conv = strtol(argv, &p, 10);

    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
		return -1;
    }

    return conv;
}

void* esperarYcalcular(){
    return 0;
}