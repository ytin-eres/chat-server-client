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

  memset(buf,0,sizeof(buf));

  if (argc != 2) usage();
  port = atoi(argv[1]);

  sockfd = socket(AF_INET,SOCK_STREAM,0);
  assert(sockfd != -1);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  res = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
  assert(res!=-1);

  res = listen(sockfd, 1);
  assert(res!=-1);

  connfd = accept(sockfd, (struct sockaddr *) &addr, (socklen_t*) &addr_len);
  assert(connfd!=-1);

  printf("Connection from %s:%d\n", inet_ntoa(addr.sin_addr), port);
  
  while(1){
    res = recv(connfd, buf, sizeof(buf), 0);
    assert(res != -1);
    
    buf[res--] = 0;
    printf("%s\n",buf);

    if(!strncmp(buf,"QUIT",4) && res==4){
      printf("Disconnected\n");
      break;
    }
    memset(buf,0,sizeof(buf));

    res = read(0,buf,MAX_BUF_LEN);
    assert(res!=-1);    
    res = send(connfd, buf, res, 0);
    assert(res!=-1);
    buf[res--] = 0;
    if(!strncmp(buf,"QUIT",4) && res==4){
      printf("Disconnected\n");
      break;
    }
    memset(buf,0,sizeof(buf));
  }
  close(sockfd);
}