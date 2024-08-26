//jun2 2024. prvi zadatak
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <malloc.h>
#include <string.h>

int N;
pthread_t nit1, nit2;
sem_t sem1, sem2, sem3;
int endSignal = 0;

void* printDivisible(void* arg){
    while (1){
        sem_wait(&sem1);
        if (endSignal) break;
        printf("%d\n", *(int*)arg);
        sem_post(&sem3);
    }
    sem_post(&sem3);

}

void* printNonDivisible(void* arg){
    while (1){
        sem_wait(&sem2);
        if (endSignal) break;
        printf("%d\n", *(int*)arg);
        sem_post(&sem3);
    }
    sem_post(&sem3);

}

int main(int argc, char* argv[]){
    int* divisibleNum = malloc(sizeof(int));
    int* nonDivisibleNum = malloc(sizeof(int));
    if (argc != 2){
        printf("Nevalidan broj argumenata.\n");
        exit(1);
    }
    N = atoi(argv[1]);

    //mislio sam da uradim sa semaforima ali sam kasnije shvatio da mi treba i sem3 kako bi prva nit 
    //cekala na ostale niti da odstampaju broj pre nego sto predje na sledecu iteraciju. 
    sem_init(&sem1, 0, -1);
    sem_init(&sem2, 0, -1);
    sem_init(&sem3, 0, 0);

    pthread_create(&nit1, NULL, &printDivisible, (void*)divisibleNum);
    pthread_create(&nit2, NULL, &printNonDivisible, (void*)nonDivisibleNum);

    for (int i = 0; i < N; i++){

        if (i % 7 == 0){
            *divisibleNum = i;
            sem_post(&sem1);
        }
        else{
            *nonDivisibleNum = i;
            sem_post(&sem2);
        }
        sem_wait(&sem3);
    }
    endSignal = 1;
    sem_post(&sem1);
    sem_post(&sem2);
    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
}