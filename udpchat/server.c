/******UDPserver*************/
#include "UDPsetup.h"

const int server_port = 6789;
const int MAXLINE = 256;
const char *LOGIN = "@LOGIN";
const char *QUIT = "@QUIT";

int  main(int argc, char const *argv[])
{
	/******sock*************/
	int server_sock = set_sock();
	/******serverinit*************/
	sockaddr_in serveraddr, clientadd;
	set_serveraddr(&serveraddr);
	int status = bind(server_sock,(sockaddr *)&serveraddr, sizeof(serveraddr));
	if( status != 0)
		err_ne("bind failure");
	/******recv*************/
	char msg_rece[MAXLINE], msg_send[MAXLINE];
	
	while(1){
		/******login insert*************/
		/******common broadast*************/
		/******quit delete*************/
	}
	return 0;
}