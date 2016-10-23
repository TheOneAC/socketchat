PROGRAM = server client1 client client0
main:
	gcc server.c -o server -lpthread
	gcc client.c -o client -lpthread
	gcc client1.c -o client1 -lpthread
	gcc client0.c -o client0 -lpthread

clean:
	rm -f $(PROGRAM)
