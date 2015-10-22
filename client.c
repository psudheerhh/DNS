#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>


void error(const char *);

struct query{                                  //structure of the packet to be queried
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


    struct query querypack;
    querypack.protocol_id=640;
    querypack.ack_no=atoi(argv[6]);
    querypack.type=atoi(argv[3]);
    querypack.r=0;
    strcpy(querypack.data,argv[4]);
    querypack.length=(sizeof(querypack));
    int verify=querypack.ack_no+1;

    


   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;

   

   if (argc != 7) 
   { 
   printf("Usage: server port\n");
    exit(1);
   }
  
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   
   
   
   if (sock < 0) 
   error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   
   n=sendto(sock,&querypack,sizeof(struct query),0,(const struct sockaddr *)&server,length);
   
   if (n < 0) 
   error("Sendto");

struct timeval timeout={atoi(argv[5]),0};


setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));


 n = recvfrom(sock,&querypack,sizeof(struct query),0,(struct sockaddr *)&from, &length);
 
   if (n < 0) 
   error("recvfrom");

    printf("%d\n",verify);

    if(querypack.ack_no==verify){            //check if server has incremented the ack_no
	   if(querypack.type==2){
		   printf(" %s \n",querypack.data);
	   }
	   else if(querypack.type==3){
		   printf(" %s \n",querypack.data);
	   }
	   else if (querypack.type==5){
		   printf(" %s \n",querypack.data);
	   }
	   else if(querypack.type==4){
		   printf("unresolved\n");
	   }

   }
   else{
	   printf("wrong type entered");

   }
   
   
   
   
   close(sock);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
