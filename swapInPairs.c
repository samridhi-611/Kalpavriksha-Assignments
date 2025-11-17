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

Node* swapPairs(Node* head) {
    Node dummy;
    dummy.next = head;

    Node* curr = &dummy;

    while (curr->next && curr->next->next) {
        Node* a = curr->next;
        Node* b = curr->next->next;

        a->next = b->next;
        b->next = a;
        curr->next = b;

        curr = a;
    }
    return dummy.next;
}

int main() {
    int arr[] = {1,2,3,4};
    Node* head = build(arr, 4);

    Node* ans = swapPairs(head);
    print(ans);
}
