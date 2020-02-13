########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic
LIBS = -lm
EJEC= ejemplo_fork ej8 ejemplo_malloc ejercicio_shell aborto

# $@: es lo que esta a la izq del ':'
# $<: es el 1º item en la lista de dependencias
# $^: es la lista de items en la lista de dependencias a la drcha ':'
########################################################
COMPRIMIR = *.c *.h Makefile *.txt
########################################################

all: $(EJEC)
tests: $(TESTS)

ejemplo_fork: ejemplo_fork.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejemplo_fork.o: ejemplo_fork.c
	$(CC) -c -o $@ $< $(CFLAGS)

ej8: ej8.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ej8.o: ej8.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejemplo_malloc: ejemplo_malloc.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejemplo_malloc.o: ejemplo_malloc.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_shell: ejercicio_shell.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_shell.o: ejercicio_shell.c
	$(CC) -c -o $@ $< $(CFLAGS)

aborto: aborto.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

aborto.o: aborto.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)



