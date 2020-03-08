########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread
LIBS = -lm
EJEC= ejercicio_captura ejercicio_captura_mejorado ejercicio_sigset ejercicio_alarm ejercicio_sem ejercicio_sem_signal ejercicio_alternar

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
	
ejercicio_alarm: ejercicio_alarm.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_alarm.o: ejercicio_alarm.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_sem: ejercicio_sem.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_sem.o: ejercicio_sem.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_sem_signal: ejercicio_sem_signal.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_sem_signal.o: ejercicio_sem_signal.c
	$(CC) -c -o $@ $< $(CFLAGS)

ejercicio_alternar: ejercicio_alternar.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ejercicio_alternar.o: ejercicio_alternar.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)

