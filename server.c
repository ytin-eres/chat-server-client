#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF_LEN 256
void usage(){
  printf("./chat-server <port>\n");
  exit(0);
}

int main(int argc, char** argv)
{
  struct sockaddr_in addr;
  int sockfd, port, res, connfd;
  socklen_t addr_len;
  char buf[MAX_BUF_LEN];

  memset(buf,0,sizeof(buf)); // Initialize buffer

  if (argc != 2) usage();
  port = atoi(argv[1]);

  sockfd = socket(AF_INET,SOCK_STREAM,0); // Initialize server socket
  assert(sockfd != -1);

  addr.sin_family = AF_INET; // Initialize sockaddr_in struct
  addr.sin_addr.s_addr = INADDR_ANY; // Initialize sockaddr_in with arbitrary IP
  addr.sin_port = htons(port); // Initialize sockaddr_in with argv port

  res = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)); // bind socket with given IP & port 
  assert(res!=-1);

  res = listen(sockfd, 1); // listen to client connect  
  assert(res!=-1);

  connfd = accept(sockfd, (struct sockaddr *) &addr, (socklen_t*) &addr_len); // accept client connect 
  assert(connfd!=-1);

  printf("Connection from %s:%d\n", inet_ntoa(addr.sin_addr), port);
  
  while(1){
    res = recv(connfd, buf, sizeof(buf), 0);  // receive server socket input
    assert(res != -1);
    
    buf[--res] = 0; // change user input \n to null
    printf("%s\n",buf);

    if(!strncmp(buf,"QUIT",4) && res==4){ // When user input exactly equals "QUIT"
      printf("Disconnected\n"); 
      break;
    }
    memset(buf,0,sizeof(buf)); // Initialize buffer

    res = read(0,buf,MAX_BUF_LEN);  // read user input
    assert(res!=-1);    
    res = send(connfd, buf, res, 0); // send to client socket
    assert(res!=-1);
    buf[--res] = 0; // change user input \n to null
    if(!strncmp(buf,"QUIT",4) && res==4){ // When user input exactly equals "QUIT"
      printf("Disconnected\n");
      break;
    }
    memset(buf,0,sizeof(buf)); // Initialize buffer
  }
  close(sockfd); // When QUIT, close fd
}