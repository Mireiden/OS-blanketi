//Kolokvijum 2023 2. Zadatak Grupa I-b
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>

int main(){

    srand(time(NULL));

    int pajp1[2], pajp2[2];
    pipe(pajp1);
    pipe(pajp2);

    if (fork() != 0){

        close(pajp1[0]);
        close(pajp2[0]);
        close(pajp2[1]);

        for (int i = 0; i < 10; ++i){
            int N = rand() % 100;
            write(pajp1[1], &N, sizeof(int));
        }

        wait(NULL);
        close(pajp1[1]);
    }

    else if (fork() != 0){

        close(pajp1[1]);
        close(pajp2[0]);

        int primljeni_N;

        for (int i = 0; i < 10; ++i){
            if (read(pajp1[0], &primljeni_N, sizeof(int)) <= 0){
                --i;
                continue;
            }
            if (primljeni_N % 2 == 0)
                primljeni_N /= 2;
            write(pajp2[1], &primljeni_N, sizeof(int));
        }

        wait(NULL);
        close(pajp1[0]);
        close(pajp2[1]);

    }

    else{

        close(pajp1[0]);
        close(pajp1[1]);
        close(pajp2[1]);

        int redirekcija = open("redirekcija.txt", O_TRUNC | O_CREAT | O_RDWR, 0777);
        dup2(redirekcija, 1);

        int primljeno_N;

        for (int i = 0; i < 10; ++i){
            if (read(pajp2[0], &primljeno_N, sizeof(int)) <= 0){
                --i;
                continue;
            }
            printf("%d\n", primljeno_N);
        }

        close(pajp2[0]);
        close(redirekcija);

    }

}