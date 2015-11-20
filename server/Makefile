CC=gcc
CFLAGS=-g
DEPS=accounts.h

server:
	$(CC) $(CFLAGS) -o server server.c accounts.c -lpthread


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm server
