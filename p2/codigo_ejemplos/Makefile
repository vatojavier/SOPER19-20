########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread
LIBS = -lm
EJEC= ejercicio_captura ejercicio_captura_mejorado ejercicio_sigset

# $@: es lo que esta a la izq del ':'
# $<: es el 1º item en la lista de dependencias
# $^: es la lista de items en la lista de dependencias a la drcha ':'
########################################################
COMPRIMIR = *.c *.h Makefile *.txt
########################################################

all: $(EJEC)
tests: $(TESTS)

ejercicio_captura: ejercicio_captura.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_captura.o: ejercicio_captura.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_captura_mejorado: ejercicio_captura_mejorado.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_captura_mejorado.o: ejercicio_captura_mejorado.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_sigset: ejercicio_sigset.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_sigset.o: ejercicio_sigset.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)

