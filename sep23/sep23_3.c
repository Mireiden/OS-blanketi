#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

#define MSGKEY_1 1001
#define MAXSIZE 255
#define ENDTYPE 999

struct msgbuf{
    long mtype;
    char mtext[MAXSIZE];
};

FILE* file;
int msqid;

int main(){
    file = fopen("poruka.txt", "r");
    if (!file){
        perror("fopen");
        return -1;
    }

    struct msgbuf message;
    msqid = msgget(MSGKEY_1, 0666 | IPC_CREAT);
    if (msqid == -1){
        perror("msgget");
        fclose(file);
        return -1;
    }

    int counter = 0;
    while (fgets(message.mtext, MAXSIZE, file)){
        ++counter;
        message.mtype = (counter % 2 == 0) ? 1 : 2;
        if (msgsnd(msqid, &message, strlen(message.mtext) + 1, 0) == -1){
            perror("msgsnd");
        }
        usleep(100000); // 100ms
    }

    // slanje poruke za zavrsetak
    message.mtype = ENDTYPE;
    strcpy(message.mtext, "end");
    if (msgsnd(msqid, &message, strlen(message.mtext) + 1, 0) == -1){
        perror("msgsnd");
    }

    fclose(file);
    return 0;
}
