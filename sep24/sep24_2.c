//TODO:zavrsiti
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define BUFFER_SIZE 1000

int pipe_fd[2];

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("argc!");
        exit(1);
    }
    pipe(pipe_fd);

    if (fork() == 0){
        close(pipe_fd[1]);
        FILE* dest = fopen(argv[2], "w");
        char buffer[BUFFER_SIZE];
        int count = 0;
        while (read(pipe_fd[0], &buffer, BUFFER_SIZE) > 0){
            for (int i = 0;i < BUFFER_SIZE;i++){
                fprintf(dest, "%d", count);
                count++;
                fprintf(dest, "%c", buffer[i]);
            }
        }
        close(pipe_fd[0]);
        fclose(dest);
    }
    else{
        FILE* src = fopen(argv[1], "r");
        if (!src){
            perror("fopen");
            exit(1);
        }
        close(pipe_fd[0]);
        char buffer[BUFFER_SIZE];
        while (!feof){
            for (int i = 0;i < BUFFER_SIZE;i++){
                char byte;
                fscanf(src, "%c", byte);
                buffer[i] = byte ^= i;
            }
            write(pipe_fd[1], &buffer, BUFFER_SIZE);
        }
        close(pipe_fd[1]);
        fclose(src);

        wait(NULL);
    }


    return 0;
}