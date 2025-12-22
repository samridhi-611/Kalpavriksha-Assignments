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
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter elements:\n");
    for (int pos = 0; pos < n; pos++)
        scanf("%d", &arr[pos]);

    FILE *fp = fopen("data.txt", "w");
    if (fp == NULL) {
        perror("File open failed");
        exit(1);
    }

    for (int pos = 0; pos < n; pos++)
        fprintf(fp, "%d ", arr[pos]);
    fclose(fp);

    printf("Before Sorting:\n");
    for (int pos = 0; pos < n; pos++)
        printf("%d ", arr[pos]);
    printf("\n");

    pid_t pid = fork();

    if (pid == 0) {
        int tempArr[n];

        fp = fopen("data.txt", "r");
        if (fp == NULL) {
            perror("File open failed");
            exit(1);
        }

        for (int pos = 0; pos < n; pos++)
            fscanf(fp, "%d", &tempArr[pos]);
        fclose(fp);

        sort(tempArr, n);

        fp = fopen("data.txt", "w");
        if (fp == NULL) {
            perror("File open failed");
            exit(1);
        }

        for (int pos = 0; pos < n; pos++)
            fprintf(fp, "%d ", tempArr[pos]);
        fclose(fp);

        exit(0);
    } else {
        wait(NULL);

        fp = fopen("data.txt", "r");
        if (fp == NULL) {
            perror("File open failed");
            exit(1);
        }

        for (int pos = 0; pos < n; pos++)
            fscanf(fp, "%d", &arr[pos]);
        fclose(fp);

        printf("After Sorting:\n");
        for (int pos = 0; pos < n; pos++)
            printf("%d ", arr[pos]);
        printf("\n");
    }

    return 0;
}
