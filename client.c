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

  memset(buf,0,sizeof(buf));

  sockfd = socket(AF_INET,SOCK_STREAM,0);
  assert(sockfd != -1);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));

  res = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
  assert(res!=-1);

  printf("Connected\n");
  
  while(1){
    res = read(0,buf,MAX_BUF_LEN);
    assert(res!=-1);    
    res = send(sockfd, buf, MAX_BUF_LEN, 0);
    assert(res!=-1);
    buf[res--] = 0;
    if(!strncmp(buf,"QUIT",4) && res==4){
      printf("Disconnected\n");
      break;
    }

    res = recv(sockfd, buf, sizeof(buf), 0);
    assert(res != -1);
    
    printf("%s\n",buf);
    buf[res--] = 0;

    if(!strncmp(buf,"QUIT",4) && res==4){
      printf("Disconnected\n");
      break;
    }

    memset(buf,0,sizeof(buf));

  }
  close(sockfd);
}