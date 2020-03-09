########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread
LIBS = -lm
EJEC= ejercicio_kill ejercicio_prottemp ejercicio_prottemp_mejorado

# $@: es lo que esta a la izq del ':'
# $<: es el 1º item en la lista de dependencias
# $^: es la lista de items en la lista de dependencias a la drcha ':'
########################################################
COMPRIMIR = *.c *.h Makefile *.txt
########################################################

all: $(EJEC)
tests: $(TESTS)

ejercicio_kill: ejercicio_kill.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_kill.o: ejercicio_kill.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
ejercicio_prottemp: ejercicio_prottemp.o prottemp.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_prottemp.o: ejercicio_prottemp.c
	$(CC) -c -o $@ $< $(CFLAGS)

prottemp.o: prottemp.c
	$(CC) -c -o $@ $< $(CFLAGS)


ejercicio_prottemp_mejorado: ejercicio_prottemp_mejorado.o prottemp_mejorado.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_prottemp_mejorado.o: ejercicio_prottemp_mejorado.c
	$(CC) -c -o $@ $< $(CFLAGS)

prottemp_mejorado.o: prottemp_mejorado.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
clean:
	rm -rf *.o $(EJEC)

