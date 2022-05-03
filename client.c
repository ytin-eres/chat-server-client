#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF_LEN 256

void usage(){
  printf("./chat-server <ip> <port>\n");
  exit(0);
}

int main(int argc, char** argv)
{
  struct sockaddr_in addr;
  int sockfd, port, res;
  char buf[MAX_BUF_LEN];

  if (argc != 3) usage();

  memset(buf,0,sizeof(buf)); // Initialize buffer

  sockfd = socket(AF_INET,SOCK_STREAM,0); // Initialize client socket
  assert(sockfd != -1);

  addr.sin_family = AF_INET; // Initialize sockaddr_in struct
  addr.sin_addr.s_addr = inet_addr(argv[1]); // Initialize sockaddr_in with argv IP
  addr.sin_port = htons(atoi(argv[2])); // Initialize sockaddr_in with argv port

  res = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)); // connect socket
  assert(res!=-1);

  printf("Connected\n");
  
  while(1){
    res = read(0,buf,MAX_BUF_LEN); // read user input 
    assert(res!=-1);
    res = send(sockfd, buf, res, 0); // send to server socket
    assert(res!=-1);
    buf[--res] = 0; // change user input \n to null
    if(!strncmp(buf,"QUIT",4) && res==4){ // When user input exactly equals "QUIT"
      printf("Disconnected\n");
      break;
    }
    memset(buf,0,sizeof(buf)); // Initialize buffer



    res = recv(sockfd, buf, sizeof(buf), 0);  // receive server socket input
    assert(res != -1);

    buf[--res] = 0;  // change user input \n to null
    printf("%s\n",buf);

    if(!strncmp(buf,"QUIT",4) && res==4){ // When user input exactly equals "QUIT"
      printf("Disconnected\n");
      break;
    }

    memset(buf,0,sizeof(buf)); // Initialize buffer
  }
  close(sockfd); // When QUIT, close fd
}