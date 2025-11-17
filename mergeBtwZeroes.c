#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node* next;
} Node;

Node* newNode(int val) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->val = val;
    n->next = NULL;
    return n;
}

Node* build(int arr[], int n) {
    Node *head = NULL, *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node* temp = newNode(arr[i]);
        if (!head) head = temp;
        else tail->next = temp;
        tail = temp;
    }
    return head;
}

void print(Node* head) {
    while (head) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");
}

Node* mergeNodes(Node* head) {
    Node* temp = head->next;
    int sum = 0;

    Node *newHead = NULL, *tail = NULL;

    while (temp) {
        if (temp->val != 0) {
            sum += temp->val;
        } else {
            Node* n = newNode(sum);

            if (!newHead) newHead = n;
            else tail->next = n;
            tail = n;

            sum = 0;
        }
        temp = temp->next;
    }
    return newHead;
}

int main() {
    int arr[] = {0,3,1,0,4,5,2,0};
    Node* head = build(arr, 8);

    Node* ans = mergeNodes(head);
    print(ans);
}
