#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} node;

node* newNode(int data) {
    node* newnode = (node*)malloc(sizeof(node));
    newnode->data = data;
    newnode->next = NULL;
    return newnode;
}

node* mergeLists(node* list1, node* list2) {
    if (!list1) return list2;
    if (!list2) return list1;

    node* dummy = (node*)malloc(sizeof(node));
    node* tail = dummy;
    dummy->next = NULL;

    while (list1 && list2) {
        if (list1->data < list2->data) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    if (list1) tail->next = list1;
    else tail->next = list2;

    node* mergedHead = dummy->next;
    free(dummy);
    return mergedHead;
}

void printList(node* head) {
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main() {
    node* list1 = newNode(1);
    list1->next = newNode(3);
    list1->next->next = newNode(5);

    node* list2 = newNode(2);
    list2->next = newNode(4);
    list2->next->next = newNode(6);

    node* merged = mergeLists(list1, list2);
    printList(merged);

    return 0;
}
