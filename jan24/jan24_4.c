//januar 2024. 4. zadatak
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <wait.h>

#define SHARED_MEM_KEY 2000
#define MUTEX_KEY 1000
#define EMPTY_KEY 1001
#define FULL_KEY 1002
#define BUFFER_SIZE 10
#define END_SIGNAL -1

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
    int mutexid, emptyid, fullid, shmemid;
    int* shmptr;
    union semun semopts;

    mutexid = semget(MUTEX_KEY, 1, 0666 | IPC_CREAT);
    emptyid = semget(EMPTY_KEY, 1, 0666 | IPC_CREAT);
    fullid = semget(FULL_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(mutexid, 0, SETVAL, semopts);
    semopts.val = BUFFER_SIZE;
    semctl(emptyid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(fullid, 0, SETVAL, semopts);

    shmemid = shmget(SHARED_MEM_KEY, BUFFER_SIZE * sizeof(int), IPC_CREAT | 0666);
    shmptr = (int*)shmat(shmemid, NULL, 0);

    if (fork() == 0){
        // Child process
        while (1){
            sem_lock(fullid);
            sem_lock(mutexid);

            if (shmptr[0] == END_SIGNAL){
                sem_unlock(mutexid);
                sem_unlock(emptyid);
                break;
            }

            int sum = 0;
            for (int i = 0; i < BUFFER_SIZE; i++){
                sum += shmptr[i];
            }
            printf("Suma procitanih brojeva: %d\n", sum);

            sem_unlock(mutexid);
            sem_unlock(emptyid);
        }
        shmdt(shmptr);
    }
    else{
        // Parent process
        FILE* file = fopen("brojevi.txt", "r");
        int broj, i;
        if (!file){
            perror("fopen");
            exit(1);
        }

        while (1){
            sem_lock(emptyid);
            sem_lock(mutexid);

            int eof_reached = 0;
            for (i = 0; i < BUFFER_SIZE; i++){
                if (fscanf(file, "%d", &broj) == 1){
                    shmptr[i] = broj;
                }
                else{
                    shmptr[i] = 0;
                    eof_reached = 1;
                }
            }
            sem_unlock(mutexid);
            sem_unlock(fullid);

            usleep(500);//jedino ovako hoce da radi jbg
            if (eof_reached){
                sem_lock(emptyid);
                sem_lock(mutexid);

                shmptr[0] = END_SIGNAL;

                sem_unlock(mutexid);
                sem_unlock(fullid);
                break;
            }
        }
        fclose(file);
        wait(NULL);

        // ciscenje
        shmdt(shmptr);
        semctl(mutexid, 0, IPC_RMID, 0);
        semctl(emptyid, 0, IPC_RMID, 0);
        semctl(fullid, 0, IPC_RMID, 0);
        shmctl(shmemid, IPC_RMID, 0);
    }

    return 0;
}
