#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

struct msg {
    long type;
    int data[50];
};

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
    key_t key = ftok("msg", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("Message queue creation failed");
        exit(1);
    }

    struct msg m;
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);
    m.data[0] = n;

    printf("Enter elements:\n");
    for (int pos = 1; pos <= n; pos++)
        scanf("%d", &m.data[pos]);

    printf("Before Sorting:\n");
    for (int pos = 1; pos <= n; pos++)
        printf("%d ", m.data[pos]);
    printf("\n");

    pid_t pid = fork();

    if (pid == 0) {   
        msgrcv(msgid, &m, sizeof(m.data), 1, 0);
        sort(&m.data[1], n);
        m.type = 2;
        msgsnd(msgid, &m, sizeof(m.data), 0);
        exit(0);
    } else {          
        m.type = 1;
        msgsnd(msgid, &m, sizeof(m.data), 0);
        wait(NULL);

        msgrcv(msgid, &m, sizeof(m.data), 2, 0);

        printf("After Sorting:\n");
        for (int pos = 1; pos <= n; pos++)
            printf("%d ", m.data[pos]);
        printf("\n");

        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}
