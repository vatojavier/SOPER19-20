########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread
LIBS = -lm
EJEC= ejemplo_fork ej8 ejemplo_malloc ejercicio_shell aborto ejercicio_shell_spawn ej3 ej4 ejemplo_descriptores ejemplo_buffer ejemplo_pipe ejemplo_hilos ejercicio_hilos

# $@: es lo que esta a la izq del ':'
# $<: es el 1º item en la lista de dependencias
# $^: es la lista de items en la lista de dependencias a la drcha ':'
########################################################
COMPRIMIR = *.c *.h Makefile *.txt
########################################################

all: $(EJEC)
tests: $(TESTS)

ej3: ej3.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ej3.o: ej3.c
	$(CC) -c -o $@ $< $(CFLAGS)

ej4: ej4.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ej4.o: ej4.c
	$(CC) -c -o $@ $< $(CFLAGS)

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

ejercicio_shell_spawn: ejercicio_shell_spawn.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_shell_spawn.o: ejercicio_shell_spawn.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejemplo_descriptores: ejemplo_descriptores.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejemplo_descriptores.o: ejemplo_descriptores.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejemplo_buffer: ejemplo_buffer.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejemplo_buffer.o: ejemplo_buffer.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejemplo_pipe: ejemplo_pipe.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejemplo_pipe.o: ejemplo_pipe.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejemplo_hilos: ejemplo_hilos.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejemplo_hilos.o: ejemplo_hilos.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_hilos: ejercicio_hilos.o hilos.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_hilos.o: ejercicio_hilos.c
	$(CC) -c -o $@ $< $(CFLAGS)

hilos.o: hilos.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)



