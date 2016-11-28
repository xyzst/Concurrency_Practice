/*
    FILE: 'proj3B_d_r273.c'
    AUTHOR: Darren Rambaud
    DATE: November 18, 2016
    DESCRIPTION:    C program which simulates the concurrent execution
                    of 2 threads using a single program:
                        Thread A: prints five subsequent lines of letter A
                        on the printer and keeps looping (~10 times)
                        Thread B: prints five subsequent lines of letter B 
                        on the printer and keeps looping (~10 times)
                    
                    NOTE: This program uses the methods in the pthread
                          package to guarantee mutual exclusion
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THREADS 2

enum { PROC0, PROC1 } volatile state = PROC0; // do not optimize 'state'

void *print();

const int LINES = 5,
          LOOP = 10;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t proceed = PTHREAD_COND_INITIALIZER;

int main() {
    pthread_t* handle = malloc(THREADS*sizeof(pthread_t));

    long j;
    for (j = 0; j < THREADS; ++j) {
        pthread_create(&handle[j], NULL, print, (void *)j);
    }
    
    for (j = THREADS - 1; j > 0; --j) {
        pthread_join(handle[j], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(handle);
    return 0;
}

void* print(void* rank) {
    register int i, // protects i & q from dereferencing
                 q; // register is a suggestion to compiler
    int my_rank = (long) rank;

    for (i = 0; i < LOOP; ++i) {
        // Waiting for the state ...
        switch (my_rank) {
            case 0:
                pthread_mutex_lock(&mutex);
                while (state != PROC0) {
                    pthread_cond_wait(&proceed, &mutex);
                }
                pthread_mutex_unlock(&mutex);
                break;
            case 1:
                pthread_mutex_lock(&mutex);
                while (state != PROC1) {
                    pthread_cond_wait(&proceed, &mutex);
                }
                pthread_mutex_unlock(&mutex);
                break;
        }

        // Print the lines of A or B, depending on state & rank
        /* Begin Critical Section */
        for (q = 0; q < LINES; ++q) {
            switch (my_rank) {
                case 0:
                    fprintf(stdout, "%d: AAAAAAAAA\n", i+1); 
                    break;
                case 1:            
                    fprintf(stdout, "%d: BBBBBBBBB\n", i+1);
                    break;
            }
        }
        /* End Critical Section */

        // Depending on rank & state, set the state to the vice-versa
        // and unblock the other thread
        switch (my_rank) {
            case 0:
                pthread_mutex_lock(&mutex);
                state = PROC1;
                pthread_cond_signal(&proceed);
                pthread_mutex_unlock(&mutex);
               break;
            case 1:
                pthread_mutex_lock(&mutex);
                state = PROC0;
                pthread_cond_signal(&proceed);
                pthread_mutex_unlock(&mutex);
                break; 
        }
        fprintf(stdout, "\n"); 
    }
    return NULL;
}
