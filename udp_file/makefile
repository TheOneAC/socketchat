PROGRAM = udp_file.h udp_file.c 
main : $(PROGRAM)
	gcc $(PROGRAM) client.c -o client -lcrypto
	gcc $(PROGRAM) server.c -o server -lcrypto
	
clean:
	rm server client
