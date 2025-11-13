#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}node;

node* newNode(int data){
    node* newnode = (node*)malloc(sizeof(node));
    newnode->data = data;
    newnode->next = NULL;
    return newnode;
}

int loopDetect(node* head){
    if(head == NULL || head->next == NULL){
        return 0;
    }
    node* slow = head;
    node* fast = head;
    int status = 0;
    while(fast!= NULL && fast->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast){
            status = 1;
            break;
        }
    }
    if(fast == NULL){
        return status;
    }
    return status;
}
int length(node* head){

    node* slow = head;
    node* fast = head;   
    while(fast!= NULL){
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast){
            break;
        }
    }
    slow = head;
    while(slow != fast){
        fast = fast->next;
        slow = slow->next;
    }
    
    int count = 1;
    fast = fast->next;
    while(fast != slow){
        fast= fast->next;
        count++ ;
    }
    return count;
}
int main(){
    node* head = newNode(1);
    head->next = newNode(2);
    head->next->next = newNode(3); 
    head->next->next->next = newNode(4); 
    head->next->next->next->next = newNode(5); 
    head->next->next->next->next->next = head->next->next; 
 
    //printf("ll created");
    int check = loopDetect(head);
    //printf("\n%d",check);
    if(check == 1){
        int val = length(head);
        printf("\nlength of loop is: %d",val);
    }
    else
    printf("\nno loop");
    return 0;
}