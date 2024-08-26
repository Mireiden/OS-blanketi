#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <wait.h>

#define SHMKEY 0x1A
#define SEMKEY1 0x2a
#define SEMKEY2 0x2b
#define BUFFER_SIZE 10

union semun{
    int val;
    struct semid_ds* buf;
    short* array;
    struct seminfo* __buf;
    void* __pad;
};

void sem_lock(int semid){
    struct sembuf sem_lock = { 0, -1, 0 };
    semop(semid, &sem_lock, 1);
}

void sem_unlock(int semid){
    struct sembuf sem_unlock = { 0, 1, 0 };
    semop(semid, &sem_unlock, 1);
}

int main(){
    int semParent, semChild, shmemid;
    int* shmptr;
    union semun semopts;

    semParent = semget(SEMKEY1, 1, 0666 | IPC_CREAT);
    semChild = semget(SEMKEY2, 1, 0666 | IPC_CREAT);

    semopts.val = 0;
    semctl(semParent, 0, SETVAL, semopts);
    semctl(semChild, 0, SETVAL, semopts);

    shmemid = shmget(SHMKEY, BUFFER_SIZE * sizeof(int), IPC_CREAT | 0666);
    shmptr = (int*)shmat(shmemid, NULL, 0);

    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }
    else if (pid == 0){
        //dete
        while (1){
            sem_lock(semChild);

            if (shmptr[0] == -1){
                sem_unlock(semParent);
                break;
            }

            int sum = 0;
            for (int i = 0; i < BUFFER_SIZE; i++){
                sum += shmptr[i];
            }
            printf("Suma procitanih brojeva je: %d\n", sum);
            sem_unlock(semParent);

        }
        shmdt(shmptr);
    }
    else{
        //roditelj
        FILE* file;
        int broj, i;
        if (!(file = fopen("brojevi.txt", "r"))){
            perror("fopen");
            exit(1);
        }

        while (1){

            int eof_reached = 0;

            for (i = 0;i < BUFFER_SIZE;i++){
                if (fscanf(file, "%d", &broj) == 1){
                    shmptr[i] = broj;
                }
                else{
                    shmptr[i] = 0;
                    eof_reached = 1;
                }
            }
            sem_unlock(semChild);
            sem_lock(semParent);
            usleep(500);

            if (eof_reached){
                shmptr[0] = -1;
                sem_unlock(semChild);
                sem_lock(semParent);
                break;
            }
        }
        fclose(file);
        wait(NULL);

        shmdt(shmptr);
        semctl(semParent, 0, IPC_RMID, 0);
        semctl(semChild, 0, IPC_RMID, 0);
        shmctl(shmemid, IPC_RMID, 0);
    }


    return 0;
}