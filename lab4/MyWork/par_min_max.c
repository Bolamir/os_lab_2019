#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "ut.h"

void Myalarm(int val)
{
    printf("Death!");
    kill(0,SIGKILL);
}
int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  signal(SIGALRM,Myalarm);
  alarm(1);
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed<=0)
                return(1);
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size<=0)
                return(1);
            break;
          case 2:
            pnum = atoi(optarg);
            printf("pnum=%d\n",pnum);
            if (pnum<=0)
                return(1);
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  int Min[2];
  int Max[2];
  FILE* m;
  FILE* M;
  if(with_files){
  m=fopen("min.txt","w");
  M=fopen("max.txt","w");
  }
  else
  {
    
    pipe(Min);
    pipe(Max);
  }


  for (int i = 0; i < pnum; i++) 
  {
    pid_t child_pid = fork();
    if (child_pid >= 0) 
    {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) 
      {
          int min=INT_MAX;
          int max=INT_MIN;
          
        for(int j=array_size/pnum*i;j<array_size/pnum*(i+1);j++)
        {
            if(array[j]>max)
                max=array[j];
            if(array[j]<min)
                min=array[j];   
        }
        
        if(with_files)
        {

        fprintf(m,"%d\n",min);
        fprintf(M,"%d\n",max);

        }
        else{
            
            close(Min[0]);
            close(Max[0]);
            write(Max[1],&max,sizeof(max));
            write(Min[1],&min,sizeof(min));
            printf("+++min====>%d\n",min);
            printf("+++max====>%d\n",max);
        }
        return 0;
        


      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
if(with_files)
{
  fclose(m);
  fclose(M);
}
  while (active_child_processes > 0) 
  {
    wait(0);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;



    if (with_files)
    {

        m=fopen("min.txt","r");
        M=fopen("max.txt","r");
        for (int i = 0; i < pnum; i++)
        {
            int min = INT_MAX;
            int max = INT_MIN;
            fscanf(m,"%d",&min);
            fscanf(M,"%d",&max);

            if (min < min_max.min) min_max.min = min;
            if (max > min_max.max) min_max.max = max;
        }
        fclose(M);
        fclose(m);
        
    } 
    else {
      close(Min[1]);
      close(Max[1]);
            
        for (int i = 0; i < pnum; i++)
        {
            int min = INT_MAX;
            int max = INT_MIN;
            read(Max[0],&max,sizeof(max));
            read(Min[0],&min,sizeof(min));
            printf("min====>%d\n",min);
            printf("max====>%d\n",max);

            if (min < min_max.min) min_max.min = min;
            if (max > min_max.max) min_max.max = max;
        }
       close(Min[0]);
       close(Max[0]);
    }

    

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
