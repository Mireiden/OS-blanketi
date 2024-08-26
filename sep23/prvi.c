#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

#define MSGKEY 1001
#define MAXSIZE 255
#define ENDTYPE 999

struct msgbuf{
    long mtype;
    char mtext[MAXSIZE];
};

int main(){
    FILE* file = fopen("sredjeno.txt", "a");
    if (!file){
        perror("fopen");
        return -1;
    }

    int msqid = msgget(MSGKEY, 0666 | IPC_CREAT);
    if (msqid == -1){
        perror("msgget");
        fclose(file);
        return -1;
    }

    struct msgbuf message;
    char buffer[MAXSIZE];

    while (1){
        if (msgrcv(msqid, &message, MAXSIZE, 0, 0) == -1){
            perror("msgrcv");
            break;
        }
        if (message.mtype == ENDTYPE){
            break;
        }
        if (message.mtype == 1){
            strcpy(buffer, message.mtext);
            for (int i = 0; buffer[i] != '\0'; i++){
                buffer[i] = toupper(buffer[i]);
            }
            fprintf(file, "%s", buffer);
            fflush(file);
        }
    }

    fclose(file);
    msgctl(msqid, IPC_RMID, NULL);
    return 0;
}
