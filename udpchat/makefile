PROGRAM = UDPsetup.h UDPsetup.c 
main : $(PROGRAM)
	gcc $(PROGRAM) client.c -o client -lpthread
	gcc $(PROGRAM) server.c -o server -lpthread
	
clean:
	rm server client