//januar 2024, prvi zadatak
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#define MAX_LINE_LENGTH 80
#define MAX_NUMBERS 50

FILE* file;
sem_t sem, printSem;
int niz[MAX_NUMBERS];
int brojac = 0;
int suma = 0;

void* getsum(void* arg){
    while (1){
        sem_wait(&sem);
        if (brojac == -1) break;

        suma = 0;
        for (int i = 0; i < brojac;++i)
            suma += niz[i];
        sem_post(&printSem);
    }
    return NULL;
}

int main(){

    pthread_t thread;
    char linija[MAX_LINE_LENGTH];

    sem_init(&sem, 0, 0);
    sem_init(&printSem, 0, 0);
    pthread_create(&thread, NULL, getsum, NULL);

    file = fopen("ulaz.txt", "r");
    if (!file){
        printf("Greska pri greska pri otvaranju fajla.");
        exit(1);
    }

    while (fgets(linija, MAX_LINE_LENGTH, file) != NULL){
        brojac = 0;
        char* token = strtok(linija, " ");
        while (token){
            niz[brojac++] = atoi(token);
            token = strtok(NULL, " ");
        }
        sem_post(&sem);
        sem_wait(&printSem);
        printf("Suma : %d\n", suma);
    }

    brojac = -1;
    sem_post(&sem);
    pthread_join(thread, NULL);
    fclose(file);
    sem_destroy(&sem);
    sem_destroy(&printSem);

    return 0;
}