#include <stdio.h>
#include <stdlib.h>
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
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter elements:\n");
    for (int pos = 0; pos < n; pos++)
        scanf("%d", &arr[pos]);

    printf("Before Sorting:\n");
    for (int pos = 0; pos < n; pos++)
        printf("%d ", arr[pos]);
    printf("\n");

    pid_t pid = fork();

    if (pid == 0) {   // Child
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], arr, sizeof(arr));
        sort(arr, n);
        write(pipe2[1], arr, sizeof(arr));

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else {          
        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], arr, sizeof(arr));
        wait(NULL);
        read(pipe2[0], arr, sizeof(arr));

        close(pipe1[1]);
        close(pipe2[0]);

        printf("After Sorting:\n");
        for (int pos = 0; pos < n; pos++)
            printf("%d ", arr[pos]);
        printf("\n");
    }

    return 0;
}
