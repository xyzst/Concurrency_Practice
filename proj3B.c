/*
    FILE: 'proj3B.c'
    AUTHOR: Darren Rambaud
    DATE: November 18, 2016
    DESCRIPTION:    C program which simulates the concurrent execution
                    of 2 threads using a single program:
                        Thread A: prints five subsequent lines of letter A
                        on the printer and keeps looping (~10 times)
                        Thread B: prints five subsequent lines of letter B 
                        on the printer and keeps looping (~10 times)
                    
                    NOTE: This program uses Peterson's algorithm to
                          guarantee mutual exclusion
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print();
void petersonLock(int);
void petersonUnlock(int);

const int LINES = 5,
          LOOP = 10;

pthread_t thr_a,
          thr_b;

// initializing lock
static volatile int flag[2] = {0, 0};
static volatile int turn = 0; 

int main() {
    pthread_create(&thr_a, NULL, print, (void*)NULL); // create thread 'A'
    pthread_create(&thr_b, NULL, print, (void*)1); // create thread 'B'
    pthread_exit((void *)NULL); // thread exits
    return 0;
}

void petersonLock(int x) {
    flag[x] = 1;
    turn = 1 - x;
    while (flag[1-x] == 1 && turn == 1 - x);
}

void petersonUnlock(int x) {
    flag[x] = 0;
}

void* print(void* k) { // to be executed by threads
    register int i,
                 q;
    int rank = (long)k;
    
    for (i = 0; i < LOOP; ++i) {
        petersonLock(rank);
        /* Critical Section */
        for (q = 0; q < LINES; ++q) {
            switch(rank) {
                case 0:
                    fprintf(stdout, "%d: AAAAAAAAA\n", i+1);
                    break;
                default:
                    fprintf(stdout, "%d: BBBBBBBBB\n", i+1);
            }
        }
        fprintf(stdout, "\n");
        /* End Critical Section */
        petersonUnlock(rank);
    }
    return NULL;
}
