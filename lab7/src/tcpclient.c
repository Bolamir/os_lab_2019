#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <signal.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int fd;
  int BUFSIZE1=-1;
  int nread;
  char* buf;
  char* src;
  struct sockaddr_in servaddr;
  int port =-1;
  
printf("That is ok -1 \n");
  
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
        port=atoi(optarg);
        if(port <0)
        {
            printf("mod mut be positiv number");
            return 1;
        }
        break;
      case 1:
        BUFSIZE1=atoi(optarg);
        if(BUFSIZE1 <0)
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
  
  if (argc < 3) {
    printf("Too few arguments %d\n",argc);
    exit(1);
  }

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  buf=(char*)malloc(sizeof(char)*BUFSIZE1);
  
  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

 

  if (inet_pton(AF_INET, argv[argc-1], &servaddr.sin_addr) <= 0) {
    printf("bad address: %s\n", argv[argc-1]);
    exit(1);
  }

  

  servaddr.sin_port = htons(port);

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }
   
  //kill(0,SIGKILL);

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, BUFSIZE1)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
    if(strchr(buf,'~')!=0)
        break;
  }

  close(fd);
  free(buf);
  exit(0);
}
