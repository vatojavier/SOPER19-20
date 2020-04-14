########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic -pthread -lrt
LIBS = -lm
EJEC= shm_concurrence shm_concurrence_solved shm_consumer shm_producer mq_injector mq_workers_pool mq_workers_pool_timed shm_producer_file shm_consumer_file

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

shm_concurrence_solved: shm_concurrence_solved.o concurrence.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_concurrence_solved.o: shm_concurrence_solved.c
	$(CC) -c -o $@ $< $(CFLAGS)

concurrence.o: concurrence.c
	$(CC) -c -o $@ $< $(CFLAGS)

shm_producer: shm_producer.o queue.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_producer.o: shm_producer.c
	$(CC) -c -o $@ $< $(CFLAGS)

shm_consumer: shm_consumer.o queue.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_consumer.o: shm_consumer.c
	$(CC) -c -o $@ $< $(CFLAGS)

shm_producer_file: shm_producer_file.o queue.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_producer_file.o: shm_producer_file.c
	$(CC) -c -o $@ $< $(CFLAGS)

shm_consumer_file: shm_consumer_file.o queue.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

shm_consumer_file.o: shm_consumer_file.c
	$(CC) -c -o $@ $< $(CFLAGS)

queue.o:queue.c
	$(CC) -c -o $@ $< $(CFLAGS)

mq_injector: mq_injector.o pool_trabajadores.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

mq_injector.o: mq_injector.c
	$(CC) -c -o $@ $< $(CFLAGS)

mq_workers_pool: mq_workers_pool.o pool_trabajadores.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

mq_workers_pool.o: mq_workers_pool.c
	$(CC) -c -o $@ $< $(CFLAGS)

mq_workers_pool_timed: mq_workers_pool_timed.o pool_trabajadores.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

mq_workers_pool_timed.o: mq_workers_pool_timed.c
	$(CC) -c -o $@ $< $(CFLAGS)

pool_trabajadores.o: pool_trabajadores.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(EJEC)

