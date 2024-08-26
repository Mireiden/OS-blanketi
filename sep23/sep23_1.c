//septembar 2023, prvi
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 10

int bafer[10];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int novaGrupa = 0;
int suma = 0;
int krajRada = 0;

void* generisi(void* arg){
    while (!krajRada){
        pthread_mutex_lock(&mutex);

        for (int i = 0; i < N; i++)
            bafer[i] = rand() % 200;

        printf("Nova grupa generisana\n");

        novaGrupa = 1;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);

        usleep(500000);
    }
    pthread_exit(NULL);
}

int main(){

    srand(time(NULL));
    pthread_t nit;

    pthread_create(&nit, NULL, &generisi, NULL);

    while (suma <= 1000){
        pthread_mutex_lock(&mutex);

        suma = 0;

        while (!novaGrupa){
            pthread_cond_wait(&cond, &mutex);
        }

        for (int i = 0; i < N; i++)
            suma += bafer[i];

        (suma > 800) ? printf("Zbir je veci od 800.\n") : printf("Zbir je manji od 800.\n");

        novaGrupa = 0;

        pthread_mutex_unlock(&mutex);
    }

    printf("Zbir je veci od 1000, izlaz\n");
    krajRada = 1;
    pthread_cancel(nit);
    pthread_join(nit, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}
