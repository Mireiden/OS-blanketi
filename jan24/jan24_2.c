//januar 2024, drugi zadatak
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <string.h>

int main(){

    int pipe_r_d[2], pipe_d_r[2];

    pipe(pipe_r_d);
    pipe(pipe_d_r);
    char recenica[255];

    printf("Unesite recenicu:\n");
    fgets(recenica, sizeof(recenica), stdin);

    //roditelj proces
    if (fork() != 0){
        close(pipe_d_r[1]);
        close(pipe_r_d[0]);

        char* buffer;
        int primljenaSuma;
        buffer = strtok(recenica, " ");

        while (buffer){
            write(pipe_r_d[1], buffer, strlen(buffer) + 1);
            read(pipe_d_r[0], &primljenaSuma, sizeof(primljenaSuma));
            printf("Suma cifara u reci %s je %d. \n", buffer, primljenaSuma);
            buffer = strtok(NULL, " ");
        }

        close(pipe_r_d[1]);
        close(pipe_d_r[0]);
        wait(NULL);
    }

    //dete proces
    else{
        close(pipe_d_r[0]);
        close(pipe_r_d[1]);

        char primljenaRec[100];

        while (read(pipe_r_d[0], primljenaRec, sizeof(primljenaRec)) > 0){
            int suma = 0;
            for (int i = 0; i < strlen(primljenaRec);++i)
                if (primljenaRec[i] >= '0' && primljenaRec[i] <= '9')
                    suma += primljenaRec[i] - '0';//pretvaranje char u int
            write(pipe_d_r[1], &suma, sizeof(suma));
        }

        close(pipe_d_r[1]);
        close(pipe_r_d[0]);
    }
}