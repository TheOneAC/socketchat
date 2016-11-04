#include "udp_file.h"

void err_msg(const char *msg){
	perror(msg);
	exit(1);
}
void serv_addr_init(sockaddr_in * serv_addr){
	bzero(serv_addr, sizeof(*serv_addr));
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr->sin_port = htons(SERVER_PORT);
}
void clit_addr_init(sockaddr_in *clit_addr){
	bzero(clit_addr, sizeof(*clit_addr));
	clit_addr->sin_family = AF_INET;
	clit_addr->sin_addr.s_addr = htons(INADDR_ANY);
	clit_addr->sin_port = htons(0);
}
void clit_serv_addr_init(sockaddr_in * clit_serv_addr,const char* ipaddr){
	bzero(clit_serv_sock,sizeof(*clit_serv_addr));
	clit_serv_addr->sin_family = AF_INET;
	clit_serv_addr->sin_port = htons(SERVER_PORT);
	if(inet_aton(ipaddr, clit_serv_addr) == 0)
		err_msg("unavailable server ip ");
}