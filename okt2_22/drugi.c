#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#define MAXLEN 255
#define MSGKEY 10002

struct msgbuf{
    long mtype;
    char mtext[MAXLEN];
};

int main(){

    int mqid;
    mqid = msgget(MSGKEY, IPC_CREAT | 0666);
    struct msgbuf buffer;
    buffer.mtype = 2;

    while (msgrcv(mqid, &buffer, MAXLEN, buffer.mtype, 0) >= 0){
        for (int i = 0; i < strlen(buffer.mtext); i++)
            if (buffer.mtext[i] >= 97 && buffer.mtext[i] <= 122)
                buffer.mtext[i] -= 32;
        printf("%s\n", buffer.mtext);
    }

}