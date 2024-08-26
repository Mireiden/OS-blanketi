#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 200

int main(int argc, char** argv){

    srand(time(NULL));

    char* file_name = argv[1];
    int pipefd[2];
    pipe(pipefd);

    FILE* inputFile = fopen(argv[1], "r");
    FILE* outputFile = fopen(argv[2], "w+");

    if (fork() != 0){
        close(pipefd[0]);

        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, inputFile))
            write(pipefd[1], buffer, BUFFER_SIZE);

        close(pipefd[1]);
        fclose(inputFile);
        wait(NULL);
    }

    else{
        close(pipefd[1]);

        char readBuffer[BUFFER_SIZE];
        while (read(pipefd[0], readBuffer, BUFFER_SIZE) > 0){
            int randomNum = (rand() & 255) + 1;

            for (int i = 0; i < BUFFER_SIZE; i++)
                readBuffer[i] ^= randomNum;

            fprintf(outputFile, "%d%s", randomNum, readBuffer);
        }

        close(pipefd[0]);
        fclose(outputFile);
    }

    return 0;
}