#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

#define MAX_BUFFER_SIZE 257

int pipe1[2], pipe2[2];

int main(){
    int pid1, pid2;
    pipe(pipe1);
    pipe(pipe2);

    // prvo dete
    pid1 = fork();
    if (pid1 < 0){
        perror("fork");
        exit(1);
    }
    else if (pid1 == 0){
        close(pipe1[1]);
        close(pipe2[0]);

        sleep(1);
        FILE* file;
        char recvbuffer[MAX_BUFFER_SIZE];
        if (read(pipe1[0], recvbuffer, MAX_BUFFER_SIZE) == -1){
            perror("read");
            exit(1);
        }
        file = fopen(recvbuffer, "r");
        if (!file){
            perror("fopen");
            exit(1);
        }

        while (read(pipe1[0], recvbuffer, MAX_BUFFER_SIZE) > 0){
            char kljucnaRec[40];
            strcpy(kljucnaRec, recvbuffer);

            char linija[MAX_BUFFER_SIZE];
            int redniBroj = 1;
            while (fgets(linija, MAX_BUFFER_SIZE, file)){
                char* rec = strtok(linija, " ");
                while (rec != NULL){
                    if (strcmp(rec, kljucnaRec) == 0){
                        if (write(pipe2[1], &redniBroj, sizeof(int)) == -1){
                            perror("write");
                            exit(1);
                        }
                        break;
                    }
                    rec = strtok(NULL, " ");
                }
                redniBroj++;
            }

            rewind(file); // vrati se na pocetak fajla za sledecu kljucnu rec
        }

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    }

    // drugo dete
    pid2 = fork();
    if (pid2 < 0){
        perror("fork");
        exit(1);
    }
    else if (pid2 == 0){
        sleep(2);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        int recvBroj;
        int suma = 0;
        while (read(pipe2[0], &recvBroj, sizeof(int)) > 0){
            suma += recvBroj;
        }

        close(pipe2[0]);
        printf("Suma linija je: %d\n", suma);
        exit(0);
    }

    // roditelj
    else{
        char buffer[MAX_BUFFER_SIZE];

        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        char* fajl = "tekst.txt";
        char* rec1 = "druga";
        char* rec2 = "respect";
        char* rec3 = "nema";
        int argc_sim = 5;
        char* argv_sim[argc_sim];
        argv_sim[1] = fajl;
        argv_sim[2] = rec1;
        argv_sim[3] = rec2;
        argv_sim[4] = rec3;

        strcpy(buffer, argv_sim[1]);

        if (write(pipe1[1], buffer, strlen(buffer) + 1) == -1){
            perror("write");
            exit(1);
        }

        for (int i = 2; i < argc_sim; i++){
            strcpy(buffer, argv_sim[i]);
            if (write(pipe1[1], buffer, strlen(buffer) + 1) == -1){ // +1 za null terminator
                perror("write");
                exit(1);
            }
        }

        close(pipe1[1]);

        wait(NULL);
        wait(NULL);
    }
    return 0;
}
