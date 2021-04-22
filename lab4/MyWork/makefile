CC=gcc
CFLAGS=-I.

all:par_min_max zombi par_sum

par_sum:
	$(CC) -o par_sum ut.o -lpthread par_sum.c $(CFLAGS)

zombi:
	$(CC) -o zombi zombi.c $(CFLAGS)

par_min_max : ut.o  ut.h 
	$(CC) -o par_min_max ut.o  par_min_max.c $(CFLAGS)

ut.o : ut.h 
	$(CC) -o ut.o -c ut.c $(CFLAGS)


clean :
	rm ut.o  par_min_max zombi par_sum
