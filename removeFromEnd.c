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

Node* removeNthFromEnd(Node* head, int n) {
    Node dummy;
    dummy.next = head;

    Node* fast = &dummy;
    Node* slow = &dummy;

    for (int i = 0; i < n; i++)
        fast = fast->next;

    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }

    slow->next = slow->next->next;
    return dummy.next;
}

int main() {
    int arr[] = {1,2,3,4,5};
    Node* head = build(arr, 5);

    Node* ans = removeNthFromEnd(head, 2);
    print(ans);
}
