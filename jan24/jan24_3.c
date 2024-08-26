#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>

#define MSGKEY_1 10001
#define MSGKEY_2 10002
#define MAXLEN 255
#define END_TYPE 999

FILE* file;

struct msgbuf{
    long mtype;
    char mtext[MAXLEN];
};


int main(int argc, char* argv[]){

    file = fopen("prijava.txt", "r");
    if (!file){
        perror("fopen");
        return -1;
    }

    int msqid1 = msgget(MSGKEY_1, 0666 | IPC_CREAT);
    int msqid2 = msgget(MSGKEY_2, 0666 | IPC_CREAT);

    if (msqid1 == -1 || msqid2 == -1)
        return -1;

    struct msgbuf mqbuffer;
    char buffer[MAXLEN];

    while (fgets(buffer, sizeof(buffer), file)){
        char podaci[MAXLEN];
        strcpy(podaci, buffer);
        char* ime = strtok(buffer, " ");
        char* prezime = strtok(NULL, " ");
        int br_indeksa = atoi(strtok(NULL, "\n"));
        strcpy(mqbuffer.mtext, podaci);

        if (br_indeksa < 18000){
            mqbuffer.mtype = 1;
            msgsnd(msqid1, &mqbuffer, strlen(mqbuffer.mtext) + sizeof(long), 0);
        }

        else{
            mqbuffer.mtype = 2;
            msgsnd(msqid2, &mqbuffer, strlen(mqbuffer.mtext) + sizeof(long), 0);
        }
        usleep(100);//sleeping operacija kako bi dodatni procesi stigli da prime poruke kako treba
    }

    fclose(file);
    mqbuffer.mtype = END_TYPE;

    printf("\nPrekidanje programa\n");
    strcpy(mqbuffer.mtext, "kraj");
    msgsnd(msqid1, &mqbuffer, strlen(mqbuffer.mtext) + sizeof(long), 0);
    msgsnd(msqid2, &mqbuffer, strlen(mqbuffer.mtext) + sizeof(long), 0);

    msgctl(msqid1, IPC_RMID, NULL);
    msgctl(msqid2, IPC_RMID, NULL);

    return 0;
}