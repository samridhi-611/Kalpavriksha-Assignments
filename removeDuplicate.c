#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} node;

node *newNode(int data)
{
    node *newnode = (node *)malloc(sizeof(node));
    newnode->data = data;
    newnode->next = NULL;
    return newnode;
}

node *deleteDuplicates(node *head)
{
    if (!head)
        return NULL;

    node *dummy = (node *)malloc(sizeof(node));
    dummy->next = head;
    node *prev = dummy;

    while (head)
    {
        int duplicate = 0;
        while (head->next && head->data == head->next->data)
        {
            head = head->next;
            duplicate = 1;
        }
        if (duplicate)
            prev->next = head->next;
        else
            prev = prev->next;
        head = head->next;
    }

    node *newHead = dummy->next;
    free(dummy);
    return newHead;
}

void printList(node *head)
{
    while (head)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main()
{
    node *head = newNode(1);
    head->next = newNode(2);
    head->next->next = newNode(3);
    head->next->next->next = newNode(3);
    head->next->next->next->next = newNode(4);
    head->next->next->next->next->next = newNode(4);
    head->next->next->next->next->next->next = newNode(5);

    head = deleteDuplicates(head);
    printList(head);

    return 0;
}
