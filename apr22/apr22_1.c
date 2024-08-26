//april 2022, prvi zadatak
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

sem_t mainSem;
sem_t funcSem;

int num;

void* count(void* arg){
    while (1){
        sem_wait(&funcSem);
        if (num == 99)
            break;
        int i;
        for (i = 0; i < num; i++){
            printf("%d\n", i);
            sleep(3);
        }
        sem_post(&mainSem);


    }
}

int main(){
    pthread_t thread;
    sem_init(&funcSem, 0, 0);
    sem_init(&mainSem, 0, 0);

    pthread_create(&thread, NULL, &count, NULL);

    printf("Unesite broj: ");
    fscanf(stdin, "%d", &num);
    while (num != 99){
        sem_post(&funcSem);
        sem_wait(&mainSem);

        printf("Unesi broj:");
        fscanf(stdin, "%d", &num);
    }

    sem_post(&funcSem);
    pthread_join(thread, NULL);

    sem_destroy(&mainSem);
    sem_destroy(&funcSem);

    return 0;

}

