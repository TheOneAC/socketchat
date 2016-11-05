#include "udp_file.h"
#define NDEBUG
int main(int argc, char const *argv[])
{
	sockaddr_in clit_addr, clit_serv_addr;
	clit_addr_init(&clit_addr);
	clit_serv_addr_init(&clit_serv_addr,argv[1]);
	int clit_sock = socket(AF_INET, SOCK_DGRAM,0);
	if(clit_sock <0)
		err_msg("clit_sock init failed");
	char filename[MAXLINE];
	send_filename(filename,clit_sock,&clit_serv_addr);
	FILE * fp = fopen(filename,"w+");
	if(fp == NULL){
		printf("file %s  ",filename);
		err_msg("cann`t open");
	}
	char buffer[MAXLINE];
	bzero(buffer,MAXLINE);
	int len = 0;
	clock_t start,end;
	start = clock();
	/**********recv file**************/
	int pack_cnt = 0;
	while(1){
		len = recv(clit_sock,buffer,MAXLINE,0);
		if(len < 0){
			err_msg("file trans failed");
		}else if(len == 0) break;
		printf("Package %d received succes!....\n",pack_cnt++);
#ifndef NDEBUG
		puts(buffer);
		printf("\n%d\n",len);
#endif
		int write_len = fwrite(buffer,sizeof(char),len,fp);
        if (write_len < len)
        	err_msg("file write failed");
        fflush(fp);  
		bzero(buffer,MAXLINE);
	}
	end = clock();
	puts("-----------------------------------------");
	printf ("file trans costs %f seconds.\n",((float)(end-start))/CLOCKS_PER_SEC);
	/**********recv remote md5**************/
	char remo_md5[33];
	bzero(remo_md5,33);
	if((recv(clit_sock,remo_md5,MAXLINE,0)) < 0)
		err_msg("file trans failed");
	printf("Remote MD5 key: %s\n",remo_md5);
	/**********create local md5**************/
	char local_md5[33];
	bzero(local_md5,33);
	Gen_md5(filename, local_md5);
	printf("Local  MD5 key: %s\n",local_md5);
	/**********comp md5**************/
	if(strncmp(local_md5,remo_md5,strlen(local_md5)) == 0)
		printf("MD5 KEY THE SAME , FILE TRANS SUCCESS!\n");
	return 0;
}