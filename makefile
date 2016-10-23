PROGRAM = server client1 client
main:
	gcc server.c -o server -lpthread
	gcc client.c -o client -lpthread
	gcc client1.c -o client1 -lpthread

clean:
	rm -f $(PROGRAM)
