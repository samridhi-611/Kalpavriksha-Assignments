#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void sort(int arr[], int n) {
    for (int left = 0; left < n - 1; left++) {
        for (int right = left + 1; right < n; right++) {
            if (arr[left] > arr[right]) {
                int temp = arr[left];
                arr[left] = arr[right];
                arr[right] = temp;
            }
        }
    }
}

int main() {
    key_t key = ftok("shm", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    if (shmid == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }

    int *shm = (int *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("Shared memory attach failed");
        exit(1);
    }

    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    shm[0] = n;

    printf("Enter elements:\n");
    for (int pos = 1; pos <= n; pos++)
        scanf("%d", &shm[pos]);

    printf("Before Sorting:\n");
    for (int pos = 1; pos <= n; pos++)
        printf("%d ", shm[pos]);
    printf("\n");

    pid_t pid = fork();

    if (pid == 0) {   // Child
        sort(&shm[1], n);
        exit(0);
    } else {          // Parent
        wait(NULL);

        printf("After Sorting:\n");
        for (int pos = 1; pos <= n; pos++)
            printf("%d ", shm[pos]);
        printf("\n");

        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
