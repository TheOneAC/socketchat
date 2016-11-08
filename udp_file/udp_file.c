#include "udp_file.h"
char *SERVER_ADDR = "127.0.0.1";
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
	bzero(clit_serv_addr,sizeof(*clit_serv_addr));
	clit_serv_addr->sin_family = AF_INET;
	clit_serv_addr->sin_port = htons(SERVER_PORT);
	if(ipaddr != NULL)inet_aton(ipaddr, &clit_serv_addr->sin_addr);
	else inet_aton(SERVER_ADDR, &clit_serv_addr->sin_addr);
	/*if(inet_aton(SERVER_ADDR, &clit_serv_addr->sin_addr) == 0)
		err_msg("unavailable server ip ");*/
}
void send_filename(char *filename,int clit_sock,sockaddr_in *clit_serv_addr){
	bzero(filename,sizeof(*filename));
	puts("please input the filenme you want :");
	fgets(filename,MAXLINE,stdin);
	sendto(clit_sock,filename,sizeof(filename),0,(struct sockaddr*)clit_serv_addr,sizeof(*clit_serv_addr));
}

void pack_res(int clit_sock,sockaddr_in *clit_serv_addr){
	char buffer[10] = "ACK";
	socklen_t n = sizeof(*clit_serv_addr) ;
	if(sendto(clit_sock,buffer,10,0,(struct sockaddr*)clit_serv_addr,n)<0)
		err_msg("send ASK failed");
}

void pack_ack(int serv_sock, sockaddr_in *clit_addr){
	char buffer[10];
	socklen_t n = sizeof(clit_addr) ;
	//if(sendto(serv_sock,buffer,10,0,(struct sockaddr*)clit_addr,n)<0)
	//	err_msg("send ASK failed");
	int len = recvfrom(serv_sock,buffer,10,0,(struct sockaddr*)clit_addr,&n);
	if(len <0)
			err_msg("recv ACK failed");
	else if(strncmp(buffer,"ACK",3) != 0)
		err_msg("ack error");
}

void Gen_md5(const char* filename, char* md5_buffer)
{  
	int fp = open(filename, O_RDONLY );
    MD5_CTX ctx = { 0 };  
    int len = 0;  
    unsigned char buffer[1024] = {0};  
    unsigned char digest[16] = {0};  
    int SIZE;  
    MD5_Init (&ctx);  
    while ((len = read (fp, buffer, MAXLINE)) > 0)  
    {  
        MD5_Update (&ctx, buffer, len);  
    }    
    MD5_Final (digest, &ctx);
    char tmp1[3] = {0};  
    for(int i = 0; i < 16; i++ )  
    {  
        sprintf(tmp1,"%02X", digest[i]);   
        strcat(md5_buffer, tmp1);   
    }  
} 