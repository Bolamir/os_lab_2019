#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  FILE* ARR;
  ARR=fopen("array.txt","w");
  for (int i = 0; i < array_size; i++) {
    array[i] = rand()%1001;
    fprintf(ARR,"%d\n",array[i]);
  }
  fclose(ARR);
}
