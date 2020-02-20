#ifndef HILOS_H
#define HILOS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>

int pasarAint(char *argv);
void* esperarYcalcular();


#endif