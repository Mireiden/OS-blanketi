#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSGKEY 10001
#define MAXLEN 255
#define END_TYPE 999

struct msgbuf{
    long mtype;
    char mtext[MAXLEN];
};

int main(){
    int msqid;
    struct msgbuf buffer;
    msqid = msgget(MSGKEY, 0666 | IPC_CREAT);


    while (1){
        if (msgrcv(msqid, &buffer, MAXLEN, 0, 0) < 0){
            perror("msgrcv");
            break;
        }
        if (buffer.mtype == END_TYPE){
            printf("A1: %s\n", buffer.mtext);
            break;
        }
        printf("A1: %s", buffer.mtext);
        fflush(stdout);
    }
    msgctl(msqid, IPC_RMID, NULL);
    exit(1);
}