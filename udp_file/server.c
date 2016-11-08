#include "udp_file.h"



int main(int argc, char const *argv[])
{
	sockaddr_in serv_addr,clit_addr;
	serv_addr_init(&serv_addr);
	int serv_sock = socket(AF_INET, SOCK_DGRAM,0);
	if(serv_sock < 0)
		err_msg("serv_sock init failed");
	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)))
		err_msg("server bind failed");
	char buffer[MAXLINE];
	socklen_t n = sizeof(clit_addr) ;
	puts("waiting for connect");
	while(1){
		char filename[MAXLINE];
		bzero(filename,sizeof(filename));
		int len = recvfrom(serv_sock,filename,MAXLINE,0,(struct sockaddr*)&clit_addr,&n);
		if(len <0)
			err_msg("recv msg failed");
		puts("client connect success");
		puts(filename);
		clock_t start,end;
		//FILE* fp =fopen(filename,"r+");
		int fp = open(filename, O_RDONLY );
		if(fp <0)
			err_msg("file not found");
		else{
			int pack_cnt=0;
			start =clock();
			/*********file trans**********/
			while(1){
				bzero(buffer,sizeof(buffer));
				len = read(fp, buffer, MAXLINE);
				if (len < 0)
					err_msg("file read failed");
				if(sendto(serv_sock,buffer,len,0,(struct sockaddr*)&clit_addr,n)<0)
					err_msg("send file failed");
				if(len == 0)break;
				//puts(buffer);
				/*********pack ask**********/
				pack_ack(serv_sock,&clit_addr);
				printf("package %d acked \n",pack_cnt++);
			}
			end = clock();
			printf ("file trans costs %f seconds.\n",((float)(end-start))/CLOCKS_PER_SEC);
			/*********Generate md5 key**********/
			char md5[33];
			bzero(md5,33);
			Gen_md5(filename, md5);
			printf("Server MD5 Key : %s\n",md5);
			/*********send md5 key**********/
			if(sendto(serv_sock,md5,strlen(md5),0,(struct sockaddr*)&clit_addr,n)<0)
				err_msg("send MD5 key failed");
		}
	}

	return 0;
}