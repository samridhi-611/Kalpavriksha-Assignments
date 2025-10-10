#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Student {
    int roll;
    char name[50];
    float marks[3];
    float total;
    float average;
    char grade;
};

float calculateTotal(float marks[]) {
    return marks[0] + marks[1] + marks[2];
}

float calculateAverage(float total) {
    return total / 3;
}

char assignGrade(float avg) {
    if (avg >= 85)
        return 'A';
    else if (avg >= 70)
        return 'B';
    else if (avg >= 50)
        return 'C';
    else if (avg >= 35)
        return 'D';
    else
        return 'F';
}

void displayPerformance(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        case 'F':
            printf("Performance: \n");
            return;
    }

    printf("Performance: ");
    for (int i = 0; i < stars; i++) {
        printf("*");
    }
    printf("\n");
}

void printRollNumbers(struct Student students[], int index, int n) {
    if (index == n)
        return;

    printf("%d", students[index].roll);
    if (index < n - 1)
        printf("  ");
    printRollNumbers(students, index + 1, n);
}

int isValidName(char name[]) {
    for (int i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]))
            return 0;
    }
    return 1;
}

void getValidName(char name[]) {
    while (1) {
        scanf("%s", name);
        if (isValidName(name))
            break;
        printf("Invalid name! Please enter again: ");
    }
}

float getValidMarks() {
    float marks;
    while (1) {
        if (scanf("%f", &marks) != 1) {
            printf("Invalid input! Enter a number between 0 and 100: ");
            while (getchar() != '\n'); // clear buffer
            continue;
        }
        if (marks >= 0 && marks <= 100)
            return marks;
        printf("Invalid marks! Enter a value between 0 and 100: ");
    }
}

int getValidRollNumber() {
    int roll;
    while (1) {
        if (scanf("%d", &roll) != 1) {
            printf("Invalid input! Enter a positive integer for roll number: ");
            while (getchar() != '\n'); 
            continue;
        }
        if (roll > 0)
            return roll;
        printf("Invalid! Roll number must be positive: ");
    }
}

int main() {
    struct Student students[100];
    int n;

    while (1) {
        printf("Enter number of students between 1 to 100: ");
        if (scanf("%d", &n) == 1 && n >= 1 && n <= 100)
            break;
        printf("Invalid number! Please enter between 1 and 100.\n");
        while (getchar() != '\n');
    }

    for (int i = 0; i < n; i++) {
        printf("\nEnter details for student %d:\n", i + 1);

        printf("Roll Number: ");
        students[i].roll = getValidRollNumber();

        printf("Name: ");
        getValidName(students[i].name);

        printf("Marks in Subject 1: ");
        students[i].marks[0] = getValidMarks();

        printf("Marks in Subject 2: ");
        students[i].marks[1] = getValidMarks();

        printf("Marks in Subject 3: ");
        students[i].marks[2] = getValidMarks();

        students[i].total = calculateTotal(students[i].marks);
        students[i].average = calculateAverage(students[i].total);
        students[i].grade = assignGrade(students[i].average);
    }

    printf("\n--- STUDENT PERFORMANCE REPORT ---\n");
    for (int i = 0; i < n; i++) {
        printf("\nRoll: %d\n", students[i].roll);
        printf("Name: %s\n", students[i].name);
        printf("Total: %.2f\n", students[i].total);
        printf("Average: %.2f\n", students[i].average);
        printf("Grade: %c\n", students[i].grade);

        if (students[i].grade == 'F') {
            printf("Performance : \n");
            continue;
        }

        displayPerformance(students[i].grade);
    }

    printf("\nList of Roll Numbers : ");
    printRollNumbers(students, 0, n);
    printf("\n");

    return 0;
}