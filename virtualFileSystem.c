#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define TOTAL_BLOCKS 1024
#define NAME_LIMIT 50
#define SIZE 100

typedef struct FreeBlock {
    int blockNumber;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct {
    FreeBlock *head;
    int freeCount;
} FreeList;

typedef struct Node {
    char name[NAME_LIMIT];
    int type; 
    struct Node *parent;
    struct Node *next;
    struct Node *prev;
    struct Node *childHead;
} Node;

FreeList freeList = {NULL, 0};
Node *root = NULL;
Node *current = NULL;

void initializeFreeBlocks() {
    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
        block->blockNumber = i;
        block->next = freeList.head;
        block->prev = NULL;
        if (freeList.head != NULL) {
            freeList.head->prev = block;
        }
        freeList.head = block;
        freeList.freeCount++;
    }
}

Node* createNode(const char *name, int type, Node *parent) {
    Node *node = (Node *)malloc(sizeof(Node));
    strcpy(node->name, name);
    node->type = type;
    node->parent = parent;
    node->next = node;
    node->prev = node;
    node->childHead = NULL;
    return node;
}

Node* findChild(Node *folder, const char *name) {
    if (folder->childHead == NULL) {
        return NULL;
    }

    Node *temp = folder->childHead;
    while (1) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
        if (temp == folder->childHead) {
            break;
        }
    }

    return NULL;
}

void addChild(Node *folder, Node *child) {
    if (folder->childHead == NULL) {
        folder->childHead = child;
    } else {
        Node *first = folder->childHead;
        Node *last = first->prev;

        last->next = child;
        child->prev = last;
        child->next = first;
        first->prev = child;
    }
}

void makeDirectory(const char *name) {
    Node *exists = findChild(current, name);
    if (exists != NULL) {
        printf("Directory already exists.\n");
    } else {
        Node *dir = createNode(name, 1, current);
        addChild(current, dir);
        printf("Directory '%s' created.\n", name);
    }
}

void makeFile(const char *name) {
    Node *exists = findChild(current, name);
    if (exists != NULL) {
        printf("File already exists.\n");
    } else {
        Node *file = createNode(name, 2, current);
        addChild(current, file);
        printf("File '%s' created.\n", name);
    }
}

void listItems() {
    if (current->childHead == NULL) {
        printf("(empty)\n");
    } else {
        Node *temp = current->childHead;
        while (1) {
            if (temp->type == 1) {
                printf("%s/\n", temp->name);
            } else {
                printf("%s\n", temp->name);
            }
            temp = temp->next;
            if (temp == current->childHead) {
                break;
            }
        }
    }
}

void printCurrentPath(Node *dir) {
    if (dir == root) {
        printf("/\n");
    } else {
        char path[1024] = "";
        while (dir != NULL && dir != root) {
            char temp[1024];
            sprintf(temp, "/%s%s", dir->name, path);
            strcpy(path, temp);
            dir = dir->parent;
        }
        printf("%s\n", path);
    }
}

void changeDirectory(const char *name) {
    if (strcmp(name, "..") == 0) {
        if (current->parent != NULL) {
            current = current->parent;
        } else {
            printf("Already at root directory.\n");
        }
    } else {
        Node *dir = findChild(current, name);
        if (dir == NULL) {
            printf("Directory not found.\n");
        } else if (dir->type != 1) {
            printf("Not a directory.\n");
        } else {
            current = dir;
        }
    }
}

void showDiskInfo() {
    printf("Total Blocks: %d\n", TOTAL_BLOCKS);
    printf("Free Blocks: %d\n", freeList.freeCount);
    printf("Used Blocks: %d\n", TOTAL_BLOCKS - freeList.freeCount);
}

void initializeFileSystem() {
    initializeFreeBlocks();
    root = createNode("/", 1, NULL);
    current = root;
    printf("Virtual File System initialized. Type 'exit' to quit.\n");
}

int main() {
    initializeFileSystem();
    char command[SIZE];
    char arg[SIZE];

    while (1) {
        printf("/ > ");
        scanf("%s", command);

        if (strcmp(command, "mkdir") == 0) {
            scanf("%s", arg);
            makeDirectory(arg);
        } 
        else if (strcmp(command, "create") == 0) {
            scanf("%s", arg);
            makeFile(arg);
        } 
        else if (strcmp(command, "ls") == 0) {
            listItems();
        } 
        else if (strcmp(command, "pwd") == 0) {
            printCurrentPath(current);
        } 
        else if (strcmp(command, "cd") == 0) {
            scanf("%s", arg);
            changeDirectory(arg);
        } 
        else if (strcmp(command, "df") == 0) {
            showDiskInfo();
        } 
        else if (strcmp(command, "exit") == 0) {
            printf("Exiting Virtual File System.\n");
            break;
        } 
        else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}
