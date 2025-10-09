#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 256  

// Function Declarations 
int getOperatorPriority(char operator);
int performOperation(int operand1, int operand2, char operator, int *flag);
int evaluateExpression(char *expression, int *flag);

int main()
{
    char expression[MAX];
    printf("Enter a math expression (e.g. 12+3*4): ");
    gets(expression);  

    int flag = 0;
    int result = evaluateExpression(expression, &flag);

    if (!flag)
        printf("Answer = %d\n", result);

    return 0;
}

// Function to get operator priority
int getOperatorPriority(char operator) {
    if (operator == '+' || operator == '-') return 1;  // Low priority
    if (operator == '*' || operator == '/') return 2;  // High priority
    return 0;  // Not an operator
}

// Function to perform calculation based on operator
int performOperation(int operand1, int operand2, char operator, int *flag) {
    switch (operator) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/':
            if (operand2 == 0) {
                printf("Error: Division by zero!\n");
                *flag = 1;
                return 0;
            }
            return operand1 / operand2;
    }
    return 0;
}

// Function to evaluate 
int evaluateExpression(char *expression, int *flag) {
    int numberStack[MAX];     
    char operatorStack[MAX];  
    int numberTop = -1, operatorTop = -1;

    for (int i = 0; expression[i] != '\0'; i++) {
        if (isspace(expression[i])) continue;  // Skip spaces

        // If current character is a number, read the full number
        if (isdigit(expression[i])) {
            int currentNumber = 0;
            while (isdigit(expression[i])) {
                currentNumber = currentNumber * 10 + (expression[i] - '0');
                i++;
            }
            i--;  
            numberStack[++numberTop] = currentNumber;  // Push number to stack
        }
        // If current character is an operator
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            while (operatorTop >= 0 && getOperatorPriority(operatorStack[operatorTop]) >= getOperatorPriority(expression[i])) {
                int operand2 = numberStack[numberTop--];
                int operand1 = numberStack[numberTop--];
                char operator = operatorStack[operatorTop--];
                numberStack[++numberTop] = performOperation(operand1, operand2, operator, flag);
                if (*flag) return 0;
            }
            operatorStack[++operatorTop] = expression[i];  // Push current operator
        }
        else {
            printf("Error: Invalid character '%c' in expression!\n", expression[i]);
            *flag = 1;
            return 0;
        }
    }

    // Perform remaining operations
    while (operatorTop >= 0) {
        int operand2 = numberStack[numberTop--];
        int operand1 = numberStack[numberTop--];
        char operator = operatorStack[operatorTop--];
        numberStack[++numberTop] = performOperation(operand1, operand2, operator, flag);
        if (*flag) return 0;
    }

    return numberStack[numberTop]; 
}
