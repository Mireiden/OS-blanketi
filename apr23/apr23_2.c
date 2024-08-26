//april 2023, 2. zadatak
//uradio copilot nisam proveravao
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc < 3){
        fprintf(stderr, "Usage: %s <program_name> <arg_for_program>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pid = fork();
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){
        // Child process
        char* args[] = { argv[1], argv[2], NULL };
        if (execvp(argv[1], args) == -1){
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else{
        // Parent process
        int status;
        wait(&status); // Wait for child process to finish and capture its exit status

        if (WIFEXITED(status)){
            printf("Child process exited with code %d\n", WEXITSTATUS(status));
        }
        else{
            printf("Child process did not exit normally\n");
        }
    }

    return 0;
}