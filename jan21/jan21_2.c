#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>

int pipeRD[2], pipeDR[2];
#define N 5

int main(){
    srand(time(NULL));
    pipe(pipeRD);
    pipe(pipeDR);
    int evenCount = 0;

    if (fork() != 0){
        int num;
        int evenNum;
        close(pipeRD[0]);
        close(pipeDR[1]);

        for (int i = 0; i < N; ++i){
            num = rand() % 100;
            printf("Generisan broj: %d\n", num);
            write(pipeRD[1], &num, sizeof(int));
        }
        close(pipeRD[1]);

        while (read(pipeDR[0], &evenNum, sizeof(int)) > 0){
            evenCount++;
        }

        printf("Broj parnih brojeva: %d\n", evenCount);
        fflush(stdout);//osiguravanje da se broj odstampa

        close(pipeDR[0]);
        wait(NULL);
    }

    else{
        close(pipeRD[1]);
        close(pipeDR[0]);

        int receivedNum;

        while (read(pipeRD[0], &receivedNum, sizeof(int)) > 0){
            printf("Primljen broj: %d\n", receivedNum);
            if (receivedNum % 2 == 0){
                write(pipeDR[1], &receivedNum, sizeof(int));
            }
        }

        close(pipeRD[0]);
        close(pipeDR[1]);
    }

    return 0;
}