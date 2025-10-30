#include <stdio.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter size of array: ");
    scanf("%d", &n);

    if (n < 2) {
        printf("Array must have at least 2 elements.\n");
        return 0;
    }

    int arr[n];
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int largest = INT_MIN, second_largest = INT_MIN;
    int smallest = INT_MAX, second_smallest = INT_MAX;

    for (int i = 0; i < n; i++) {
        int val = arr[i];
        if (val > largest) {
            second_largest = largest;
            largest = val;
        } else if (val > second_largest && val != largest) {
            second_largest = val;
        }

        if (val < smallest) {
            second_smallest = smallest;
            smallest = val;
        } else if (val < second_smallest && val != smallest) {
            second_smallest = val;
        }
    }

    if (second_largest == INT_MIN || second_smallest == INT_MAX)
        printf("All elements are same or not enough unique values.\n");
    else
        printf("Second Largest: %d\nSecond Smallest: %d\n", second_largest, second_smallest);

    return 0;
}
