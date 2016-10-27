PROGRAM = server client1 client client0
main:
	gcc setup.h setup.c server.c -o server -lpthread
	gcc setup.h setup.c client.c -o client -lpthread
	gcc setup.h setup.c client1.c -o client1 -lpthread
	gcc setup.h setup.c client0.c -o client0 -lpthread

clean:
	rm -f $(PROGRAM)
