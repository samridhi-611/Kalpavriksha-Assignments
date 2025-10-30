#include <stdio.h>

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int A[n], B[n];
    printf("Enter elements of array A: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &A[i]);

    int *src = A;
    int *dest = B;

    for (int i = 0; i < n; i++)
        *(dest + i) = *(src + i);

    printf("Array A: ");
    for (int i = 0; i < n; i++)
        printf("%d ", *(src + i));

    printf("\nArray B: ");
    for (int i = 0; i < n; i++)
        printf("%d ", *(dest + i));

    printf("\n");
    return 0;
}
