

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
struct response{                                                          //structure of the response packet
 unsigned int protocol_id:16;
 unsigned int ack_no:16;
 unsigned int type:3;
 unsigned int r:1;
 unsigned int unused:12;
 unsigned int length:16;
 char data[150];
 };


int main(int argc, char *argv[])
{
   int flag=0;
   char str[100];
   char one[100];
   char two[100];
   char three[100];
   int sock, length, n;
   socklen_t clilen;
   struct sockaddr_in server;
   struct sockaddr_in cli_addr;
   struct response r_packet;
   struct response *p;
   p=&r_packet;

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }

   sock=socket(AF_INET, SOCK_DGRAM, 0);
   
   if (sock < 0) 
   error("Opening socket");
   
   length = sizeof(server);
   
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   
   if (bind(sock,(struct sockaddr *)&server,length)<0)
       error("binding");
   
   clilen = sizeof(struct sockaddr_in);
  
       n = recvfrom(sock,&r_packet,sizeof(struct response),0,(struct sockaddr *)&cli_addr,&clilen);
       
       if (n < 0) 
       error("recvfrom");
       
       write(1," \n",21);
       FILE *fd;
       FILE *fm;


       if (p->type==0||p->type==1){              //checking the type of packet and searching the appropriate response
    	   p->ack_no++;
    	   fd= fopen(argv[2],"r+");
    	   if (fd==NULL){
    		   printf("mapping file does not exixt");
    	   }
    	  while(!feof(fd)){                                            //mapping file
    		  fgets(str,100,fd);
    		  sscanf(str,"%s %s",one,two);
    		  if(strcmp(one,p->data)==0){
    		    strcpy(p->data,two);
    		    p->type=3;
    		    flag=1;
    		    break;

    		  }
    		  else if (strcmp(two,p->data)==0){
    			  strcpy(p->data,one);
    			  p->type=2;
    			  flag=1;
    			  break;
    		  }
    	  }
    	   fclose(fd);
    	if(flag==0){
    	   fm =fopen(argv[3],"r+");                                                 //redirect file
    	   if(fm==NULL){
    		   printf("redirect file does not exixt");
    	   }

    	   while(!feof(fm)){
    		   fgets(str,100,fm);
    		   sscanf(str,"%s %s %s",one,two,three);
    		   if(strcmp(one,p->data)==0){
    			   
    			   strcat(two,three);
    			   strcpy(p->data,two);
    			   p->type=5;
    			   flag=1;
    			   
    			   break;
    		   }
    	   }
    	   fclose(fm);
       }
    	if (flag==0){
    		p->type=4;
    	}
       }
       else if(p->type==6){
    	   printf("\n");
    	   exit(0);
       }
    
       n=sendto(sock,&r_packet,sizeof(struct response),0,(struct sockaddr *)&cli_addr,clilen);

    
       if (n  < 0) error("sendto");
  
       close(sock);
   return 0;
 }
