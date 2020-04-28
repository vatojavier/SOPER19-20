/**
 * @file sort_multiple.c
 *
 * Implementación de las funciones de ordenación para multiple procesos
 *
 * @author Antonio Javier Casado antonioj.casado@estudiante.uam.es
 * @author Aurora Pérez Lázaro aurora.perezl@estudiante.uam.es
 * @date 28/4/2020
 *
 */

#include "sort.h"

Status sort_multiple_process(char *file_name, int n_levels, int n_processes, int delay){
    int i, j;
    Sort sort;

    /* The data is loaded and the structure initialized. */
    if (init_sort(file_name, &sort, n_levels, n_processes, delay) == ERROR) {
        fprintf(stderr, "sort_single_process - init_sort\n");
        return ERROR;
    }

    plot_vector(sort.data, sort.n_elements);
    printf("\nStarting algorithm with %d levels and %d processes...\n", sort.n_levels, sort.n_processes);
    /* For each level, and each part, the corresponding task is solved. */
    for (i = 0; i < sort.n_levels; i++) {
        for (j = 0; j < get_number_parts(i, sort.n_levels); j++) {
            solve_task(&sort, i, j);
            plot_vector(sort.data, sort.n_elements);
            printf("\n%10s%10s%10s%10s%10s\n", "PID", "LEVEL", "PART", "INI", \
                "END");
            printf("%10d%10d%10d%10d%10d\n", getpid(), i, j, \
                sort.tasks[i][j].ini, sort.tasks[i][j].end);
        }
    }

    plot_vector(sort.data, sort.n_elements);
    printf("\nAlgorithm completed\n");

    return OK;
}

