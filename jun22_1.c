//Jun 2022 1. Zadatak
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#define N 100

int buffer_current=-1;

int buffer[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //kao pthread_mutex_init(&mutex, NULL);

void* sortiraj(void* arg){
    
    pthread_mutex_lock(&mutex);
    
    for(int i = 0; i<buffer_current-1; ++i)
        for(int j = i+1; j > buffer_current; ++j)
            if(buffer[i] < buffer[j]){
                int help = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = help;
            }
    
    pthread_mutex_unlock(&mutex);
    
    return NULL;

}


int main(){

    pthread_t thread;
    pthread_create(&thread, NULL, &sortiraj, NULL);
    
    FILE* fajl = fopen("ulaz.txt", "r");

    while(!feof(fajl)){
        pthread_mutex_lock(&mutex);
        fscanf(fajl, "%d", &buffer[++buffer_current]);
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(thread, NULL);
    
    for(int i=0;i<buffer_current;++i)
        printf("%d\n", buffer[i]);

    fclose(fajl);
    pthread_mutex_destroy(&mutex);
    return 0;

}
