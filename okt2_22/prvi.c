#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define MAXLEN 255
#define MSGKEY 10001

struct msgbuf{
    long mtype;
    char mtext[MAXLEN];
};

int main(){

    int mqid;
    struct msgbuf buffer;
    buffer.mtype = 1;
    mqid = msgget(MSGKEY, IPC_CREAT | 0666);

    while (msgrcv(mqid, &buffer, MAXLEN, buffer.mtype, 0) >= 0){
        printf("%s %d\n", buffer.mtext, strlen(buffer.mtext) - 1);
        fflush(stdout);
    }
}