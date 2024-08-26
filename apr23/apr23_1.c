#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 10
#define MAX_ITERATIONS 5

int buffer[BUFFER_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condProducer = PTHREAD_COND_INITIALIZER, condMain = PTHREAD_COND_INITIALIZER;
int bufferReady = 0, producerDone = 0, stopThread = 0;

void* generateNumbersParity(void* arg){
    while (1){
        pthread_mutex_lock(&mutex);
        while (!bufferReady && !stopThread){
            pthread_cond_wait(&condProducer, &mutex);
        }
        if (stopThread){
            pthread_mutex_unlock(&mutex);
            break;
        }
        for (int i = 0; i < BUFFER_SIZE / 2; i++){
            buffer[i * 2] = rand() % 300; // Generiše broj na parnim indeksima
        }
        producerDone++;
        if (producerDone == 2){
            pthread_cond_signal(&condMain);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* generateNumbersNonParity(void* arg){
    while (1){
        pthread_mutex_lock(&mutex);
        while (!bufferReady && !stopThread){
            pthread_cond_wait(&condProducer, &mutex);
        }
        if (stopThread){
            pthread_mutex_unlock(&mutex);
            break;
        }
        for (int i = 0; i < BUFFER_SIZE / 2; i++){
            buffer[i * 2 + 1] = 300 + rand() % 200; // Generiše broj na neparnim indeksima
        }
        producerDone++;
        if (producerDone == 2){
            pthread_cond_signal(&condMain);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    pthread_t parityThread, nonParityThread;
    srand(time(NULL));

    pthread_create(&parityThread, NULL, generateNumbersParity, NULL);
    pthread_create(&nonParityThread, NULL, generateNumbersNonParity, NULL);

    for (int i = 0; i < MAX_ITERATIONS; i++){
        pthread_mutex_lock(&mutex);
        bufferReady = 1;
        producerDone = 0;
        pthread_cond_broadcast(&condProducer);
        while (producerDone < 2){
            pthread_cond_wait(&condMain, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        int sum = 0;
        for (int j = 0; j < BUFFER_SIZE; j++){
            sum += buffer[j];
        }

        printf("Zbir brojeva u baferu je: %d, %s veci od 2000.\n", sum, sum > 2000 ? "" : "nije");

        bufferReady = 0; // Resetujemo za sledeću iteraciju
    }

    // Signaliziramo niti da završe
    pthread_mutex_lock(&mutex);
    stopThread = 1;
    pthread_cond_broadcast(&condProducer);
    pthread_mutex_unlock(&mutex);

    pthread_join(parityThread, NULL);
    pthread_join(nonParityThread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condProducer);
    pthread_cond_destroy(&condMain);

    return 0;
}