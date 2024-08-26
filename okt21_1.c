//Oktobar 2021. 1. Zadatak
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void* sort(void* arg){

    int broj_reci = 0;
    char reci[10][20];
    char* argument = (char*)arg;

    /* Podela na reci */
    char* rec = strtok(argument, " ");
    strcpy(reci[broj_reci++], rec);
    while ((rec = strtok(NULL, " ")))
        strcpy(reci[broj_reci++], rec);

    /* Leksikografsko sortiranje */
    for (int i = 0;i < broj_reci - 1;++i)
        for (int j = i + 1;j < broj_reci;++j)
            if (strcmp(reci[i], reci[j]) > 0){
                char help[10];
                strcpy(help, reci[i]);
                strcpy(reci[i], reci[j]);
                strcpy(reci[j], help);
            }

    /* Sklapanje recenice */
    strcpy(argument, reci[0]);
    for (int i = 1;i < broj_reci;++i){
        strcat(argument, " ");
        strcat(argument, reci[i]);
    }

    strcpy(arg, argument);

    return NULL;
}

void* print(void* arg){

    /* Pretvaranje prvog slova u veliko */
    char* argument = (char*)arg;
    if (argument[0] > 96 && argument[0] < 123)
        argument[0] -= 32;

    /* Dodavanje tacke na kraj */
    int argument_velicina = strlen(argument);

    argument[argument_velicina + 1] = '\0';
    argument[argument_velicina] = '.';

    printf("%s\n", argument);

    return NULL;
}

int main(){

    char recenica[100];
    fgets(recenica, 100, stdin);
    recenica[strlen(recenica) - 1] = '\0';

    pthread_t thread_sort, thread_print;

    pthread_create(&thread_sort, NULL, &sort, recenica);
    pthread_join(thread_sort, NULL);
    pthread_create(&thread_print, NULL, &print, recenica);
    pthread_join(thread_sort, NULL);

    printf("KRAJ\n");

    return 0;

}