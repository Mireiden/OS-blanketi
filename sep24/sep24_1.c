//septembar 2024. prvi zadatak
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>

pthread_t nit;
sem_t semWorker, semMain;

void* worker(void* arg){
    while (1){
        sem_wait(&semWorker);
        int n = *(int*)arg;
        for (int i = 0;i <= n;i++){
            printf("%d\n", i);
            sleep(1);
        }
        sem_post(&semMain);
        if (n == 999) break;
    }
}

int main(){

    sem_init(&semMain, 0, 1);
    sem_init(&semWorker, 0, 0);

    int* num = malloc(sizeof(int));
    pthread_create(&nit, NULL, worker, (void*)num);

    while (1){
        sem_wait(&semMain);
        printf("unesite broj: \n");
        scanf("%d", num);

        sem_post(&semWorker);
        if (*num == 999){
            break;
        }
    }
    return 0;
}