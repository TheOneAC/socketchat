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


void Gen_md5(const char* filename, char* md5_buffer)
{  
	FILE *fp = fopen(filename,"r+");
    MD5_CTX ctx = { 0 };  
    int len = 0;  
    unsigned char buffer[1024] = {0};  
    unsigned char digest[16] = {0};  
    int SIZE;  
    MD5_Init (&ctx);  
    while ((len = fread (buffer, 1, MAXLINE, fp)) > 0)  
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