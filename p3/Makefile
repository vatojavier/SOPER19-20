########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread -lrt
LIBS = -lm
EJEC= shm_concurrence

# $@: es lo que esta a la izq del ':'
# $<: es el 1º item en la lista de dependencias
# $^: es la lista de items en la lista de dependencias a la drcha ':'
########################################################
COMPRIMIR = *.c *.h Makefile *.txt
########################################################
#valgrind --leak-check=full prog

all: $(EJEC)
tests: $(TESTS)

shm_concurrence: shm_concurrence.o concurrence.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_concurrence.o: shm_concurrence.c
	$(CC) -c -o $@ $< $(CFLAGS)

concurrence.o: concurrence.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)

