//oktobar 2 2022. prvi zadatak
//gde je sinhronizacija??? ne valja
//TODO:sinhronizacija
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int num;

void* count(void* arg){
    int n = *(int*)arg;
    for (int i = n - 1; i >= 0; --i){
        sleep(1);
        printf("%d\n", i);
    }
    return NULL;
}

int main(){
    pthread_t thread;
    char input[6];
    fgets(input, 6, stdin);
    input[4] = '\0';

    while (strcmp(input, "KRAJ") != 0){
        int num;
        if ((num = strtol(input, NULL, 10)) == 0){
            fprintf(stderr, "Greska, uneta vrednost ne moze biti konvertovana u broj.");
            return EXIT_FAILURE;
        }
        pthread_create(&thread, NULL, &count, &num);
        pthread_join(thread, NULL);

        fgets(input, 6, stdin);
        input[4] = '\0';

    }
    return 0;

}