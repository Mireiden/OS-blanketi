#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/msg.h>
#include <wait.h>
#include <string.h>

#define MSGKEY 1001
#define MAX_RANGE 127
#define EXIT_VALUE 0
#define EXIT_TYPE 999
#define MAX_BUFFER_SIZE 10

struct msgbuf{
    long mtype;
    char mtext[MAX_BUFFER_SIZE];
};

int msqid;

int main(){
    msqid = msgget(MSGKEY, 0666 | IPC_CREAT);
    if (msqid == -1){
        perror("msgget");
        exit(1);
    }

    struct msgbuf buffer;
    if (fork() == 0){
        int counter = 0;

        while (1){
            if (msgrcv(msqid, &buffer, MAX_BUFFER_SIZE, 0, 0) == -1){
                perror("msgrecv");
                exit(1);
            }
            counter++;
            if (buffer.mtype == EXIT_TYPE){
                printf("Primljena nula, zatvaram proces dete.\n");
                fflush(stdout);
                break;
            }
            if (buffer.mtype == 1){
                int broj = atoi(buffer.mtext);
                printf("Proces dete: primljeni broj je %d, ASCII vrednost broja je %c\n", broj, (char)broj);
                fflush(stdout);
            }
        }
        printf("Ukupno unetih brojeva: %d\n", counter);
        fflush(stdout);
    }
    else{
        int broj;
        while (1){
            buffer.mtype = 0;
            scanf("%d", &broj);
            if (broj == EXIT_VALUE) break;
            else if (broj > MAX_RANGE){
                printf("Uneli ste broj veci od %d\n", MAX_RANGE);
                continue;
            }
            buffer.mtype = 1;
            sprintf(buffer.mtext, "%d", broj);
            msgsnd(msqid, &buffer, MAX_BUFFER_SIZE, 0);
        }

        buffer.mtype = EXIT_TYPE;
        msgsnd(msqid, &buffer, MAX_BUFFER_SIZE, 0);
        msgctl(msqid, IPC_RMID, NULL);
        wait(NULL);

    }
    return 0;
}