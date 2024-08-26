#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGKEY 0x1A

struct msgbuf{
    long mtype;
    int mtext;
};

int main(){
    key_t msqid = msgget(MSGKEY, 0666 | IPC_CREAT);
    srand(time(NULL));

    if (fork() == 0){
        struct msgbuf recvbuf;
        while (1){
            msgrcv(msqid, &recvbuf, sizeof(recvbuf.mtext), 1, 0);

            if (recvbuf.mtext == -1){
                printf("Proces dete primio kod za izlazak %d.\n", recvbuf.mtext);
                break;
            }
            printf("Dete primilo broj: %d\n", recvbuf.mtext);
        }

        exit(0);
    }
    else{
        struct msgbuf sendbuf;
        int N = 10; //dali su da N bude random ali to je malo glupo jer je RAND_MAX = 2147483647

        for (int i = 0;i < N;i++){
            int num = rand();
            sendbuf.mtype = 1;
            sendbuf.mtext = num;
            if (msgsnd(msqid, &sendbuf, sizeof(sendbuf.mtext), 0) < 0){
                perror("msgsnd");
                exit(1);
            }
            sendbuf.mtype = 0;
        }

        sendbuf.mtype = 1;
        sendbuf.mtext = -1;
        if (msgsnd(msqid, &sendbuf, sizeof(sendbuf.mtext), 0) < 0){
            perror("msgsnd");
            exit(1);
        }

        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
    }
    return 0;
}