#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    if (argc < 2){
        fprintf(stderr, "Usage: %s <command> <arg1> <arg2> ... <argN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t childPid = fork();

    if (childPid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (childPid == 0){

        // Obrnuto slanje argumenata detetu koristeći execl sistemski poziv
        char* args[argc + 1];  // +1 za NULL terminator
        args[0] = argv[1];     // Prvi argument je naziv programa
        for (int i = argc - 1; i > 1; i--){
            args[argc - i] = argv[i];
        }
        args[argc] = NULL;  // Poslednji argument treba biti NULL

        execvp(argv[1], args);

        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Roditelj: Kreiran je proces deteta sa PID-om %d.\n", childPid);

        wait(NULL);

        printf("Roditelj: Proces deteta završen.\n");
    }

    return 0;
}
