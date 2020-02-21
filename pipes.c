/**
 * @file pipes.c
 *
 * Implementación de libreria de pipes para el ultimo ejercicio
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @date 21/2/2020
 */

#include "pipes.h"


int generar_numero(){
    

    srand(time(NULL));
    return random() % 1001;
}