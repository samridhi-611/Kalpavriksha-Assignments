#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 256  // Maximum size for the input expression

// Function Declarations 
int getPriority(char op);
int calculate(int a, int b, char op, int *error);
int solve(char *exp, int *error);

int main()
{
    char exp[MAX];
    printf("Enter a math expression (e.g. 12+3*4): ");
    gets(exp); 

    int flag = 0;
    int result = solve(exp, &flag);

    if (!flag)
        printf("Answer = %d\n", result);

    return 0;
}

// Function to get operator priority
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;  // Low priority
    if (op == '*' || op == '/') return 2;  // High priority
    return 0;  // Not an operator
}

// Function to perform calculation based on operator
int calculate(int a, int b, char op, int *error) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                printf("Error: Division by zero!\n");
                *error = 1;
                return 0;
            }
            return a / b;
    }
    return 0;
}

// Function to evaluate the mathematical expression using 2 stacks
int solve(char *exp, int *error) {
    int numbers[MAX];     // Stack to store numbers
    char operators[MAX];  // Stack to store operators
    int numTop = -1, opTop = -1;

    for (int i = 0; exp[i] != '\0'; i++) {
        if (isspace(exp[i])) continue;  // Skip spaces

        // If current character is a number  ---> read the full number.
        if (isdigit(exp[i])) {
            int num = 0;
            while (isdigit(exp[i])) {
                num = num * 10 + (exp[i] - '0');
                i++;
            }
            i--;  
            numbers[++numTop] = num;  // Push number to stack
        }
        // If current character is an operator
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            while (opTop >= 0 && getPriority(operators[opTop]) >= getPriority(exp[i])) {
                int b = numbers[numTop--];
                int a = numbers[numTop--];
                char op = operators[opTop--];
                numbers[++numTop] = calculate(a, b, op, error);
                if (*error) return 0;  // Stop if error occurs
            }
            operators[++opTop] = exp[i];  // Push current operator
        }
        else {
            printf("Error: Invalid character '%c' in expression!\n", exp[i]);
            *error = 1;
            return 0;
        }
    }

    // Perform remaining operations
    while (opTop >= 0) {
        int b = numbers[numTop--];
        int a = numbers[numTop--];
        char op = operators[opTop--];
        numbers[++numTop] = calculate(a, b, op, error);
        if (*error) return 0;
    }

    return numbers[numTop]; 
}
