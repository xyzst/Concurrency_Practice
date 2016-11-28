/*
    FILE: 'proj3A.c'
    AUTHOR: Darren Rambaud
    DATE: November 18, 2016
    DESCRIPTION:    C program which simulates the concurrent execution
                    of 2 threads using a single program:
                        Thread A: prints five subsequent lines of letter A
                        on the printer and keeps looping (~10 times)
                        Thread B: prints five subsequent lines of letter B 
                        on the printer and keeps looping (~10 times)
                    
                    NOTE: This program does not use any algorithm for mutual
                          exclusion
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 2

void *printAoB();

const int LINES = 5,
          LOOP = 10;


int main() {
    long q;
    pthread_t* thr = malloc(THREADS*sizeof(pthread_t));
  
   for (q = 0; q < THREADS; ++q) {
        pthread_create(&thr[q], NULL, printAoB, (void *)q); //create threads
    }

    for (q = THREADS - 1; q > 0; --q) {
        pthread_join(thr[q], NULL);
    }

    free(thr);
    return 0;
}

void *printAoB(void* rank) {
    register int i, 
                 x;
    int my_rank = (long)rank;
    for (x = 0; x < LOOP; ++x) {
        for (i = 0; i < LINES; ++i) {
            if (my_rank == 0) {
                fprintf(stdout, "%d: AAAAAAAAA\n", x+1);
            }
            else {
                fprintf(stdout, "%d: BBBBBBBBB\n", x+1);
            }
        }
        fprintf(stdout, "\n");
    }
    return NULL;
}
