PROGRAM = setup.h setup.c
server cleint :$(PROGRAM)
	gcc setup.h setup.c UDPclient.c -o client
	gcc setup.h setup.c UDPclient.c -o client1
	gcc setup.h setup.c UDPserver.c -o server
	./server
	
.PHONY : clean
clean: 
	rm client server
