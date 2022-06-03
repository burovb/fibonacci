CC=gcc
CFLAGS=-c -Wall
EXECUTABLE=server client log

all: $(EXECUTABLE)
	rm -rf *.o

server: fib.o server.o
	$(CC) $^ -lpthread -o $@

client: client.o
	$(CC) client.o -o client

log: log.o
	$(CC) log.o -o log

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)