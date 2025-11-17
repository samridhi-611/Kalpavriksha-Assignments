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

int findPeakElement(int nums[], int n) {
    int l = 0, r = n - 1;

    while (l < r) {
        int mid = (l + r) / 2;

        if (nums[mid] < nums[mid + 1])
            l = mid + 1;
        else
            r = mid;
    }
    return l;
}

int main() {
    int nums[] = {1,2,3,1};
    printf("%d", findPeakElement(nums, 4));
}
