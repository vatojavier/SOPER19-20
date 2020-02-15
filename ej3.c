#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char** argv){
    FILE *f;
    int nErr;

    f = fopen(argv[1], "r");
    if(f == NULL){
        nErr = errno;
        printf("Y su numero de error es %d\n", nErr);
        perror("");
    }else{
        fclose(f);
    }

    

}