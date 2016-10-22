PROGRAM = server clientA
main:
	gcc server.c -o server -lpthread
	gcc client.c -o client -lpthread
	gcc client.c -o client1 -lpthread

clean:
	rm -f $(PROGRAM)
