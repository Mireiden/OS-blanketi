#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

sem_t semMain, semWorker;
char input[6];
int value;
int krajIzvrsenja = 0;

void* print(void* args){

    while (!krajIzvrsenja){
        sem_wait(&semMain);
        if (krajIzvrsenja) break; // Proveri opet da li je kraj izvrsenja
        for (int i = value; i >= 0; --i){
            printf("%d\n", i);
            sleep(2); // Pauza od 2 sekunde izmedju brojeva
        }
        sem_post(&semWorker);
    }

    return NULL;

}

int main(){
    pthread_t child_thread;

    sem_init(&semMain, 0, 0);
    sem_init(&semWorker, 0, 1);
    pthread_create(&child_thread, NULL, &print, NULL);

    while (1){
        fgets(input, 6, stdin);
        input[strcspn(input, "\n")] = '\0'; // Ukloni novi red sa kraja stringa
        if (strcmp(input, "KRAJ") == 0){
            krajIzvrsenja = 1;
            sem_post(&semMain); // Probudi nit da zavrsi
            break;
        }
        if ((value = strtol(input, NULL, 10)) == 0){
            fprintf(stderr, "Vrednost uneta ne moze biti konvertovana u broj\n");
            continue;
        }
        sem_post(&semMain); // Probudi radnu nit
        sem_wait(&semWorker); // Sacekaj da radna nit zavrsi
    }

    pthread_join(child_thread, NULL);

    sem_destroy(&semMain);
    sem_destroy(&semWorker);

    return EXIT_SUCCESS;
}
