#include "udp_file.h"

int main(int argc, char const *argv[])
{
	if(argc != 2)
		err_msg("./client serveripaddress");
	sockaddr_in clit_addr, clit_serv_addr;
	clit_addr_init(&clit_addr);
	clit_serv_addr_init(&clit_serv_sock,argv[1]);
	int clit_sock = socket(AF_INET, SOCK_DGRAM,0);
	if(clit_sock <0)
		err_msg("clit_sock init failed");
	char filename[MAXLINE];
	send_filename(filename,clit_sock,&clit_serv_addr);
	FILE * fp = fopen(filename,"w");
	if(fp == NULL){
		printf("file %s  ",filename);
		err_msg("cann`t open");
	}
	char buffer[MAXLINE];
	bzero(buffer,MAXLINE);
	/**********recv file**************/
	while(){

	}
	/**********recv md5**************/


	/**********create local md5**************/

	/**********comp md5**************/
	
	return 0;
}