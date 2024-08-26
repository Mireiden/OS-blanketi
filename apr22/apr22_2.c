//april 2022, drugi zadatak
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

#define N 10

int main(){
    int pipe_r_d[2], pipe_d_r[2];
    pipe(pipe_r_d);
    pipe(pipe_d_r);

    srand(time(NULL));

    if (fork() == 0){
        //dete
        close(pipe_r_d[1]);
        close(pipe_d_r[0]);

        int childBuffer;
        while (read(pipe_r_d[0], &childBuffer, sizeof(childBuffer)) > 0){
            printf("Dete proces primio broj: %d\n", childBuffer);
            if (childBuffer % 3 == 0){
                childBuffer += 25;
                write(pipe_d_r[1], &childBuffer, sizeof(childBuffer));
            }
        }
        close(pipe_r_d[0]);
        close(pipe_d_r[1]);
    }
    else{
        //roditelj
        close(pipe_r_d[0]);
        close(pipe_d_r[1]);

        for (int i = 0;i < N;i++){
            int randomNum = rand() % 100 + 200;
            write(pipe_r_d[1], &randomNum, sizeof(randomNum));
        }

        usleep(500);
        close(pipe_r_d[1]);

        int buffer;
        while (read(pipe_d_r[0], &buffer, sizeof(buffer)) > 0)
            printf("Primljen broj: %d\n", buffer);

        close(pipe_d_r[0]);
        wait(NULL);
    }


    return 0;
}