//oktobar 2018. prvi
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

sem_t sem;
FILE* izlaz;

#define MAX_LINE_LENGTH 80

void* Prva(void* arg){
    char* fajl = (char*)arg;
    FILE* ulaz = fopen(fajl, "r");
    if (ulaz == NULL){
        perror("Greška pri otvaranju datoteke za čitanje");
        exit(EXIT_FAILURE);
    }

    char linija[MAX_LINE_LENGTH];
    int i = 1;

    while (fgets(linija, MAX_LINE_LENGTH, ulaz) != NULL){
        linija[strlen(linija) - 1] = '\0'; // Ukloni newline karakter

        sem_wait(&sem);

        izlaz = fopen("izlaz.txt", "a");
        if (izlaz == NULL){
            perror("Greška pri otvaranju izlazne datoteke za pisanje");
            exit(EXIT_FAILURE);
        }

        fprintf(izlaz, "%s:%d:%s\n", fajl, i, linija);

        fclose(izlaz);

        sem_post(&sem);
        i++;
    }

    fclose(ulaz);
    pthread_exit(NULL);
}

void* Druga(void* arg){
    char* fajl = (char*)arg;
    FILE* ulaz = fopen(fajl, "r");
    if (ulaz == NULL){
        perror("Greška pri otvaranju datoteke za čitanje");
        exit(EXIT_FAILURE);
    }

    char linija[MAX_LINE_LENGTH];
    int i = 1;

    while (fgets(linija, MAX_LINE_LENGTH, ulaz) != NULL){
        linija[strlen(linija) - 1] = '\0'; // Ukloni newline karakter

        sem_wait(&sem);

        izlaz = fopen("izlaz.txt", "a");
        if (izlaz == NULL){
            perror("Greška pri otvaranju izlazne datoteke za pisanje");
            exit(EXIT_FAILURE);
        }

        fprintf(izlaz, "%s:%d:%s\n", fajl, i, linija);

        fclose(izlaz);

        sem_post(&sem);
        i++;
    }

    fclose(ulaz);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    pthread_t nit1;
    pthread_t nit2;

    if (argc < 3){
        fprintf(stderr, "Upotreba: %s <datoteka1> <datoteka2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sem_init(&sem, 0, 1);

    pthread_create(&nit1, NULL, Prva, (void*)argv[1]);
    pthread_create(&nit2, NULL, Druga, (void*)argv[2]);

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    sem_destroy(&sem);

    return 0;
}