#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#define SADDR struct sockaddr

int main(int argc, char *argv[]) {
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  char* buf;
  int BUFSIZE1;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  int port=-1;


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
  buf=malloc(sizeof(char)*BUFSIZE1); 
  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    printf("Those is ok 0");

    while ((nread = read(cfd, buf, BUFSIZE1+1)) > 0) {
      write(1, buf, nread);
      //if(strchr(buf,'~')!=0)
       //  break;
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    free(buf);

    close(cfd);

  }
}
