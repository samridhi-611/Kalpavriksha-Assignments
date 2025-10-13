#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NUM_SUBJECTS 3
#define MAX_STUDENTS 100

struct Student
{
    int roll;
    char name[50];
    float marks[NUM_SUBJECTS];
    float total;
    float average;
    char grade;
};

float calculateTotal(float marks[])
{
    return marks[0] + marks[1] + marks[2];
}

float calculateAverage(float total)
{
    return total / NUM_SUBJECTS;
}

char assignGrade(float avg)
{
    if (avg >= 85)
    {
        return 'A';
    }
    else if (avg >= 70)
    {
        return 'B';
    }
    else if (avg >= 50)
    {
        return 'C';
    }
    else if (avg >= 35)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}

void displayPerformance(char grade)
{
    int stars = 0;

    switch (grade)
    {
    case 'A':
        stars = 5;
        break;
    case 'B':
        stars = 4;
        break;
    case 'C':
        stars = 3;
        break;
    case 'D':
        stars = 2;
        break;
    default:
        stars = 0; 
    }
    printf("Performance: ");
    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }
    printf("\n");
}
void clearInputBuffer()
{
    while (getchar() != '\n');
}

int isValidName(char name[])
{
    int length = strlen(name);
    for (int i = 0; i < length; i++)
    {
        if (!isalpha(name[i]))
        {
            return 0;
        }
    }
    return 1;
}

void getValidName(char name[])
{
    while (1)
    {
        scanf("%s", name);
        if (isValidName(name))
            break;
        printf("Invalid name! Please enter again: ");
    }
}

float getValidMarks()
{
    float marks;
    while (1)
    {
        if (scanf("%f", &marks) != 1)
        {
            printf("Invalid input! Enter a number between 0 and 100: ");
            clearInputBuffer();
            continue;
        }
        if (marks >= 0 && marks <= 100)
            return marks;
        printf("Invalid marks! Enter a value between 0 and 100: ");
    }
}

int getValidRollNumber()
{
    int roll;
    while (1)
    {
        if (scanf("%d", &roll) != 1)
        {
            printf("Invalid input! Enter a positive integer for roll number: ");
            clearInputBuffer();
            continue;
        }
        if (roll > 0)
            return roll;
        printf("Invalid! Roll number must be positive: ");
    }
}

void printRollNumbers(struct Student students[], int index, int totalStudents)
{
    if (index == totalStudents)
        return;

    printf("%d", students[index].roll);
    if (index < totalStudents - 1)
        printf(" ");
    printRollNumbers(students, index + 1, totalStudents);
}

int main()
{
    struct Student students[MAX_STUDENTS];
    int numStudents;

    while (1)
    {
        printf("Enter number of students between 1 to %d: ", MAX_STUDENTS);
        if (scanf("%d", &numStudents) == 1 && numStudents >= 1 && numStudents <= MAX_STUDENTS)
            break;
        printf("Invalid number! Please enter between 1 and %d.\n", MAX_STUDENTS);
        clearInputBuffer();
    }

    for (int studentIndex = 0; studentIndex < numStudents; studentIndex++)
    {
        printf("\nEnter details for student %d:\n", studentIndex + 1);

        printf("Roll Number: ");
        students[studentIndex].roll = getValidRollNumber();

        printf("Name: ");
        getValidName(students[studentIndex].name);

        for (int subjectIndex = 0; subjectIndex < NUM_SUBJECTS; subjectIndex++)
        {
            printf("Marks in Subject %d: ", subjectIndex + 1);
            students[studentIndex].marks[subjectIndex] = getValidMarks();
        }

        students[studentIndex].total = calculateTotal(students[studentIndex].marks);
        students[studentIndex].average = calculateAverage(students[studentIndex].total);
        students[studentIndex].grade = assignGrade(students[studentIndex].average);
    }

    printf("\n--- STUDENT PERFORMANCE REPORT ---\n");
    for (int studentIndex = 0; studentIndex < numStudents; studentIndex++)
    {
        printf("\nRoll: %d\n", students[studentIndex].roll);
        printf("Name: %s\n", students[studentIndex].name);
        printf("Total: %.2f\n", students[studentIndex].total);
        printf("Average: %.2f\n", students[studentIndex].average);
        printf("Grade: %c\n", students[studentIndex].grade);

        if (students[studentIndex].grade == 'F')
        {
            printf("Performance: \n");
            continue;
        }

        displayPerformance(students[studentIndex].grade);
    }

    printf("\nList of Roll Numbers: ");
    printRollNumbers(students, 0, numStudents);
    printf("\n");

    return 0;
}
