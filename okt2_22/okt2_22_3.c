//Okt2 2022 3.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define MAXLEN 255

#define MSGKEY1 10001
#define MSGKEY2 10002

struct msgbuf{
    long mtype;
    char mtext[MAXLEN];
};

int main(){
    int mqid1, mqid2;
    struct msgbuf buffer;

    mqid1 = msgget(MSGKEY1, IPC_CREAT | 0666);
    mqid2 = msgget(MSGKEY2, IPC_CREAT | 0666);

    int type;
    char text[MAXLEN + 1];

    while (1){
        scanf("%d", &type);
        fgets(text, MAXLEN + 1, stdin);
        text[MAXLEN - 1] = '\0';

        if (type == 1){
            buffer.mtype = type;
            strcpy(buffer.mtext, text);

            msgsnd(mqid1, &buffer, strlen(buffer.mtext), 0);
        }

        else if (type == 2){
            buffer.mtype = type;
            strcpy(buffer.mtext, text);

            msgsnd(mqid2, &buffer, strlen(buffer.mtext), 0);
        }

        else{
            printf("Prekidanje programa\n");
            msgsnd(mqid1, &buffer, -1, 0);
            msgsnd(mqid2, &buffer, -1, 0);
            break;
        }
    }
    msgctl(mqid1, IPC_RMID, NULL);
    msgctl(mqid2, IPC_RMID, NULL);
    return 0;
}
