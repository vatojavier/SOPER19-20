########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread
LIBS = -lm
EJEC= ejercicio_arbol ejercicio_shell aborto ejercicio_shell_spawn ejercicio_hilos ejercicio_pipes

# $@: es lo que esta a la izq del ':'
# $<: es el 1º item en la lista de dependencias
# $^: es la lista de items en la lista de dependencias a la drcha ':'
########################################################
COMPRIMIR = *.c *.h Makefile *.txt
########################################################

all: $(EJEC)
tests: $(TESTS)

ejercicio_arbol: ejercicio_arbol.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_arbol.o: ejercicio_arbol.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_shell: ejercicio_shell.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_shell.o: ejercicio_shell.c
	$(CC) -c -o $@ $< $(CFLAGS)

aborto: aborto.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

aborto.o: aborto.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_shell_spawn: ejercicio_shell_spawn.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_shell_spawn.o: ejercicio_shell_spawn.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_hilos: ejercicio_hilos.o hilos.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_hilos.o: ejercicio_hilos.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_pipes: ejercicio_pipes.o pipes.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_pipes.o: ejercicio_pipes.c
	$(CC) -c -o $@ $< $(CFLAGS)

hilos.o: hilos.c
	$(CC) -c -o $@ $< $(CFLAGS)

pipes.o: pipes.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)



