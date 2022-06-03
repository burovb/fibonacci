#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define KEY 112837

struct msg_buffer {
    long msg_type;
    int msg;
} message[8];



int main() {
    int msgid = msgget(KEY, 0666 | IPC_CREAT);
    int bytes, i = 0;

    while ((bytes = msgrcv(msgid, (message + i), sizeof message, 1, IPC_NOWAIT)) > 0)
        i++;
    
    int shmid, *shm;
    for (int j = 0; j < i; j++) {
        shmid = shmget((message + j)->msg, sizeof(int *), 0666 | IPC_CREAT);
        shm = shmat(shmid, (void *)0, 0);
        printf("#%d. %d\n", (j + 1), *shm);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}