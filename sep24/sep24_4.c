//septembar 2024. 4. zadatak

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>

#define SHMKEY 0x1A
#define SEMKEY1 0x2a
#define SEMKEY2 0x2b
#define SEMKEY3 0x2c

#define N 3

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
    int semParentReady, semChildReady, semMutex, shmemid;
    int* shmptr;

    semParentReady = semget(SEMKEY1, 1, 0666 | IPC_CREAT);
    semChildReady = semget(SEMKEY2, 1, 0666 | IPC_CREAT);
    semMutex = semget(SEMKEY3, 1, 0666 | IPC_CREAT);

    union semun semopts;

    semopts.val = 0;
    semctl(semParentReady, 0, SETVAL, semopts);
    semctl(semChildReady, 0, SETVAL, semopts);
    semopts.val = 1;
    semctl(semMutex, 0, SETVAL, semopts);

    shmemid = shmget(SHMKEY, N * sizeof(int), 0666 | IPC_CREAT);
    shmptr = (int*)shmat(shmemid, NULL, 0);

    for (int i = 0; i < N; i++){
        shmptr[i] = 0;
    }

    if (fork() == 0){
        // Dete
        while (1){
            sem_lock(semParentReady);

            sem_lock(semMutex);
            int sum = 0;
            for (int i = 0; i < N; i++){
                sum += shmptr[i];
            }
            shmptr[0] = sum;
            sem_unlock(semMutex);

            sem_unlock(semChildReady);

            if (sum == 0){
                break;
            }
        }
        exit(0);

    }
    else{
        // Roditelj
        while (1){
            printf("Unesite %d brojeva: ", N);
            sem_lock(semMutex);
            for (int i = 0; i < N; i++){
                scanf("%d", &shmptr[i]);
            }
            sem_unlock(semMutex);

            sem_unlock(semParentReady);

            sem_lock(semChildReady);

            sem_lock(semMutex);
            int sum = shmptr[0];
            sem_unlock(semMutex);

            printf("Zbir je: %d\n", sum);
            if (sum == 0)
                break;
        }

        wait(NULL);
    }

    shmdt(shmptr);
    shmctl(shmemid, IPC_RMID, NULL);
    semctl(semParentReady, 0, IPC_RMID, semopts);
    semctl(semChildReady, 0, IPC_RMID, semopts);
    semctl(semMutex, 0, IPC_RMID, semopts);

    return 0;
}
