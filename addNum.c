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

Node* addTwoNumbers(Node* l1, Node* l2) {
    Node* dummy = newNode(0);
    Node* curr = dummy;
    int carry = 0;

    while (l1 || l2 || carry) {
        int sum = carry;
        if (l1) sum += l1->val, l1 = l1->next;
        if (l2) sum += l2->val, l2 = l2->next;

        carry = sum / 10;

        curr->next = newNode(sum % 10);
        curr = curr->next;
    }
    return dummy->next;
}

int main() {
    int a1[] = {2,4,3};
    int a2[] = {5,6,4};

    Node* l1 = build(a1, 3);
    Node* l2 = build(a2, 3);

    Node* ans = addTwoNumbers(l1, l2);
    print(ans);
}
