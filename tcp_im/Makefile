# File: Makefile

CFLAGS = -Wall -g -o
PROGRAM = lancher waiter

main:
	gcc waiter.c $(CFLAGS) waiter -lpthread
	gcc lancher.c $(CFLAGS) lancher -lpthread

clean:
	rm -f $(PROGRAM)
