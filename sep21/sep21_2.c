//Septembar 2021. 2. Zadatak
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <time.h>

#define N 10

int main(){

    srand(time(NULL));

    int buffer[N];

    int pajp_r_d[2], pajp_d_r[2];

    pipe(pajp_d_r);
    pipe(pajp_r_d);

    if (fork() != 0){

        /* Prvi deo */
        close(pajp_d_r[1]);
        close(pajp_r_d[0]);

        for (int i = 0;i < N;++i)
            buffer[i] = rand() % 100;

        write(pajp_r_d[1], buffer, N * sizeof(int));


        /* Treci deo */
        int novi_buffer[N];
        read(pajp_d_r[0], novi_buffer, N * sizeof(int));

        for (int i = 0;i < N;++i){
            if (novi_buffer[i] == -1)
                printf("Paran broj\n");
            else
                printf("%d\n", novi_buffer[i]);
        }

        close(pajp_d_r[0]);
        close(pajp_r_d[1]);
        wait(NULL);

    }
    else{

        /* Drugi deo */
        close(pajp_r_d[1]);
        close(pajp_d_r[0]);

        int primljeni_buffer[N];

        read(pajp_r_d[0], primljeni_buffer, N * sizeof(int));

        for (int i = 0;i < N;++i)
            if (primljeni_buffer[i] % 2 == 0)
                primljeni_buffer[i] = -1;

        write(pajp_d_r[1], primljeni_buffer, N * sizeof(int));

        close(pajp_d_r[1]);
        close(pajp_r_d[0]);

    }

}