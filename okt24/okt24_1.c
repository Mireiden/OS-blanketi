#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <malloc.h>

pthread_t t_sort, t_upper;
sem_t s_upper;

void* sort(void* arg){
    char* recenica =
        sem_post(&s_upper);
}

void* upper(void* arg){


    sem_wait(&s_upper);
}

int main(){
    char* recenica = malloc(250 * sizeof(char));
    fgets(recenica, 250, stdin);
    sem_init(&s_upper, 0, 0);
    pthread_create(&t_sort, NULL, sort, (void*)recenica);
    pthread_create(&t_upper, NULL, upper, (void*)recenica);
    pthread_join(t_sort, NULL);
    pthread_join(t_upper, NULL);

    printf("KRAJ\n");




    return 0;
}