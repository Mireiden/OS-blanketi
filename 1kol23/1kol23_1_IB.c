//1. kolokvijum 2023, grupa 1B prvi zadatak.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

FILE* file;
volatile int N;
sem_t semInput, semPrint;

void* printFromFile(void* arg){
    file = fopen("data.txt", "r");
    char buffer[128];

    while (1){
        sem_wait(&semInput);
        if (N == 99) break;

        file = fopen("data.txt", "r");
        if (!file){
            perror("fopen");
            exit(1);
        }

        for (int i = 0;i < N && !feof(file); ++i){
            if (fgets(buffer, sizeof(buffer), file) != NULL){
                printf("%s", buffer);
                sleep(1);
            }
        }
        fclose(file);
        sem_post(&semPrint);
    }
    return NULL;
}

int main(){
    pthread_t nit;
    sem_init(&semPrint, 0, 0);
    sem_init(&semInput, 0, 0);

    nit = pthread_create(&nit, NULL, &printFromFile, NULL);

    while (1){
        printf("Unesite broj: \n");
        scanf("%d", &N);

        if (N == 99) break;

        sem_post(&semInput);
        sem_wait(&semPrint);
    }

    sem_post(&semInput);//Oslobadja nit u slucaju da je na cekanju da bi mogla da se zavrsi.
    pthread_join(nit, NULL);

    sem_destroy(&semPrint);
    sem_destroy(&semInput);

    return 0;
}