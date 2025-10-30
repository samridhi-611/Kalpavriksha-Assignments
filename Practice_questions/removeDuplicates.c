#include <stdio.h>

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int arr[n];

    printf("Enter elements: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int *p = arr;
    int newSize = n;

    for (int i = 0; i < newSize; i++) {
        for (int j = i + 1; j < newSize; j++) {
            if (*(p + i) == *(p + j)) {
                for (int k = j; k < newSize - 1; k++)
                    *(p + k) = *(p + k + 1);
                newSize--;
                j--;
            }
        }
    }

    printf("Array after removing duplicates: ");
    for (int i = 0; i < newSize; i++)
        printf("%d ", *(p + i));

    printf("\nNew length: %d\n", newSize);
    return 0;
}
