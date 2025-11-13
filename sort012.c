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

node *sort(node *head)
{
    if(head == NULL){
        return NULL;
    }
    node *zerohead = newNode(-1);
    node *onehead = newNode(-1);
    node *twohead = newNode(-1);

    node *zero = zerohead;
    node *one = onehead;
    node *two = twohead;

    node *temp = head;
    while (temp)
    {
        if (temp->data == 0)
        {
            zero->next = temp;
            zero = temp;
        }
        else if (temp->data == 1)
        {
            one->next = temp;
            one = temp;
        }
        else
        {
            two->next = temp;
            two = temp;
        }
        temp = temp->next;
    }
    if (onehead->next)
        zero->next = onehead->next;
    else
        zero->next = twohead->next;

    if (twohead->next)
        one->next = twohead->next;
    else
        one->next = NULL;

    two->next = NULL;

    node *list = zerohead->next;
    free(zerohead);
    free(onehead);
    free(twohead);
    return list;
}

void printll(node *head)
{
    while (head)
    {
        printf("%d\t", head->data);
        head = head->next;
    }
    return;
}

int main()
{
    node *head = newNode(0);
    head->next = newNode(2);
    head->next->next = newNode(1);
    head->next->next->next = newNode(0);
    head->next->next->next->next = newNode(2);

    printll(head);
    node *listHead = sort(head);
    printf("\n");
    printll(listHead);
    return 0;
}