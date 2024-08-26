#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#define MSGKEY 1001
#define ENDTYPE 999
key_t msqid;

struct msgbuf{
    long mtype;
    int mtext;
};

int main(){

    if ((msqid = msgget(MSGKEY, 0666 | IPC_CREAT)) == -1){
        perror("msgget");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }

    if (pid == 0){
        struct msgbuf message;
        FILE* file;
        if (!(file = fopen("fajl.txt", "w"))){
            perror("fopen");
            exit(1);
        }
        while (1){
            if (msgrcv(msqid, &message, sizeof(message.mtext), 0, 0) < 0){
                perror("msgrcv");
                exit(1);
            }
            if (message.mtype == ENDTYPE){
                printf("Primljen kod za zavrsetak, dete izlazi\n");
                break;
            }
            if (message.mtype == 1){
                int sum = 0;
                int num = message.mtext;
                fprintf(file, "Primljen broj: %d, ", num);
                while (num != 0){
                    sum += num % 10;
                    num /= 10;
                }
                fprintf(file, "suma cifara broja: %d\n", sum);
            }
            fflush(stdout);//moram da prestanem da zaboravljam fflush ode mi dva sata na ispravljanje koda koji radi savrseno
        }
    }

    else{
        //parent
        struct msgbuf message;
        message.mtype = 0;
        int num;
        do{
            fscanf(stdin, "%d", &num);
            if (num >= 100 && num < 1000){
                message.mtype = 1;
                message.mtext = num;
                if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1){
                    perror("msgsnd");
                    exit(1);
                }
            }

        } while (num != 0);

        message.mtype = ENDTYPE;
        message.mtext = 0;
        if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1){
            perror("msgsnd");
            exit(1);
        }

        if (msgctl(msqid, IPC_RMID, NULL) == -1){
            perror("msgctl");
            exit(1);
        }

        wait(NULL);
    }

    return 0;
}
