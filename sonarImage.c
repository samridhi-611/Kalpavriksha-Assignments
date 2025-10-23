#include <stdio.h>
#define MAX 10

void displayMatrix(int *matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%4d ", *(matrix + row * size + col));
        }
        printf("\n");
    }
}

void rotate90Clockwise(int *matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = row + 1; col < size; col++) {
            int *element1 = matrix + row * size + col;
            int *element2 = matrix + col * size + row;
            int temp = *element1;
            *element1 = *element2;
            *element2 = temp;
        }
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size / 2; col++) {
            int *left = matrix + row * size + col;
            int *right = matrix + row * size + (size - col - 1);
            int temp = *left;
            *left = *right;
            *right = temp;
        }
    }
}

void applySmoothingFilter(int *matrix, int size) {
    int temp[MAX * MAX];

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            int sum = 0, count = 0;

            for (int neighborRow = row - 1; neighborRow <= row + 1; neighborRow++) {
                for (int neighborCol = col - 1; neighborCol <= col + 1; neighborCol++) {
                    if (neighborRow >= 0 && neighborRow < size && neighborCol >= 0 && neighborCol < size) {
                        sum += *(matrix + neighborRow * size + neighborCol);
                        count++;
                    }
                }
            }
            *(temp + row * size + col) = sum / count;
        }
    }

    for (int index = 0; index < size * size; index++) {
        *(matrix + index) = *(temp + index);
    }
}

int main() {
    int matrixSize;
    printf("Enter matrix size (2 - 10): ");
    scanf("%d", &matrixSize);

    if (matrixSize < 2 || matrixSize > 10) {
        printf("Invalid matrix size.\n");
        return 0;
    }

    int sonarMatrix[MAX][MAX];
    int *matrixPtr = &sonarMatrix[0][0];

    printf("Enter %d X %d matrix elements:\n", matrixSize, matrixSize);
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            scanf("%d", (matrixPtr + row * matrixSize + col));
        }
    }

    printf("\nOriginal Matrix:\n");
    displayMatrix(matrixPtr, matrixSize);

    rotate90Clockwise(matrixPtr, matrixSize);
    printf("\nMatrix after 90 degree Clockwise Rotation:\n");
    displayMatrix(matrixPtr, matrixSize);

    applySmoothingFilter(matrixPtr, matrixSize);
    printf("\nMatrix after Applying 3 X 3 Smoothing Filter:\n");
    displayMatrix(matrixPtr, matrixSize);

    return 0;
}
