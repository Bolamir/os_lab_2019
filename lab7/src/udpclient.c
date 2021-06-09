#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<signal.h>


#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
  int sockfd, n;
  int SERV_PORT=-1;
  int BUFSIZE=-1;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"port", required_argument, 0, 0},
                                      {"bs", required_argument, 0, 0},
                                     
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        SERV_PORT=atoi(optarg);
        if(SERV_PORT <0)
        {
            printf("mod mut be positiv number");
            return 1;
        }
        break;
      case 1:
        BUFSIZE=atoi(optarg);
        if(BUFSIZE <0)
        {
            printf("mod mut be positiv number");
            return 1;
        }

        break;
      
     
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  char* sendline, *recvline;
  sendline=(char*)malloc(sizeof(char)*BUFSIZE);
  recvline=(char*)malloc(sizeof(char)*(BUFSIZE+1));

  //if (argc != 2) {
   // printf("usage: client <IPaddress of server>\n");
   // exit(1);
  //}

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);

  if (inet_pton(AF_INET, argv[argc-1], &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);
  //kill(0,SIGKILL);

  while ((n = read(0, sendline, BUFSIZE)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, BUFSIZE, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
  free(sendline);
  free(recvline);
}
