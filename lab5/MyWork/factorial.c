/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *  Modified by A.Kostin
 ********************************************************
 * mutex.c
 *
 * Simple multi-threaded example with a mutex lock.
 */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

void factorial(int *);
void do_wrap_up(int);
int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;



int main(int argc, char **argv) {



int pnum = 0;
int k = 0;
int mod = 0;


while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            
            break;
          case 1:
            mod = atoi(optarg);
            if (mod<0)
                return(1);
            break;
          case 2:
            pnum = atoi(optarg);
            
            if (pnum<=0)
                return(1);
            break;
          

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      
      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

if ( mod == 0 || k == 0 || pnum == 0) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --threads_num \"num\" \n",
           argv[0]);
    return 1;
  }
  pthread_t thread[pnum];
  int en[3];
  en[0]=1;

for(int i=0;i<pnum;i++){
    
    en[1]=i*mod/pnum+1;
    
    en[2]=(i+1)*mod/pnum+1;
  if (pthread_create(&thread[i], NULL, (void *)factorial,
			  (void *)&en) != 0) {
    perror("pthread_create");
    exit(1);
  }
  if (pthread_join(thread[i], NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }
}


  

  do_wrap_up(en[0]);

  return 0;
}

void factorial(int *en) {
pthread_mutex_lock(&mut);
  int i;
  printf("Start =%d   end= %d\n",*(en+1),*(en+2)-1) ;
  int work=1;
  
  for (i = *(en+1); i < *(en+2); i++) {
    work *= i;               /* long cycle */  
  }
  
  
  *en *= work; /* write back */
  pthread_mutex_unlock(&mut);
}



void do_wrap_up(int counter) 
{
  printf("All done, factorial = %d\n", counter);
}
