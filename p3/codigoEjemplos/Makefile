########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread -lrt
LIBS = -lm
EJEC= shm_writer shm_reader

# $@: es lo que esta a la izq del ':'
# $<: es el 1º item en la lista de dependencias
# $^: es la lista de items en la lista de dependencias a la drcha ':'
########################################################
COMPRIMIR = *.c *.h Makefile *.txt
########################################################
#valgrind --leak-check=full prog

all: $(EJEC)
tests: $(TESTS)

shm_writer: shm_writer.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_writer.o: shm_writer.c
	$(CC) -c -o $@ $< $(CFLAGS)

shm_reader: shm_reader.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_reader.o: shm_reader.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)

