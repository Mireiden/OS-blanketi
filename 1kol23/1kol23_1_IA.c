//prvi kolokiujum 2023. 1. zadatak grupa I-a
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


sem_t main, worker;
FILE* file;
int N;

void* printRandom(void* arg){
    while (1){
        sem_wait(&worker);

        if (N == 99) break;

        fprintf(file, "%d\n", N);
        for (int i = 0; i < N; i++){
            char randomChar = 'A' + (rand() % 26);
            fprintf(file, "%c", randomChar);
        }
        fprintf(file, "\n");

        sem_post(&main);
    }
    return NULL;
}

void main(){
    pthread_t nit;
    sem_init(&main, 0, 0);
    sem_init(&worker, 0, 0);

    file = fopen("data.txt", "w");
    if (file == NULL){
        printf("Greska pri otvaranju fajla!\n");
        exit(1);
    }

    srand(time(NULL));

    pthread_create(&nit, NULL, &printRandom, NULL);

    do{
        printf("Unesite broj: ");
        scanf("%d", &N);

        sem_post(&worker);
        if (N != 99) sem_wait(&main);
    } while (N != 99);

    pthread_join(nit, NULL);
    fclose(file);

    sem_destroy(&main);
    sem_destroy(&worker);

}