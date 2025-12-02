#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->next = NULL;
    return node;
}

struct Node* rotateRight(struct Node* head, int k) {
    if (!head || !head->next || k == 0)
        return head;

    struct Node* temp = head;
    int len = 1;
    while (temp->next) {
        temp = temp->next;
        len++;
    }

    temp->next = head;
    k = k % len;
    int newHead = len - k;

    while (newHead--) {
        temp = temp->next;
    }

    head = temp->next;
    temp->next = NULL;
    return head;
}

void printList(struct Node* head) {
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main() {
    struct Node* head = newNode(1);
    head->next = newNode(2);
    head->next->next = newNode(3);
    head->next->next->next = newNode(4);
    head->next->next->next->next = newNode(5);

    int k;
    scanf("%d",&k);
    head = rotateRight(head, k);
    printList(head);
    return 0;
}
