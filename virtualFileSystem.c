#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define TOTAL_BLOCKS 1024
#define NAME_LIMIT 50
#define INPUT_SIZE 1024

char fileData[TOTAL_BLOCKS][BLOCK_SIZE];

typedef struct FreeBlock {
    int blockNumber;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct {
    FreeBlock *head;
    FreeBlock *tail;
    int freeCount;
} FreeList;

typedef struct Node {
    char name[NAME_LIMIT];
    int type;
    struct Node *parent;
    struct Node *next;
    struct Node *prev;
    struct Node *childHead;
    int *allocatedBlocks;
    int numBlocks;
    int fileSize;
} Node;

FreeList freeList = {NULL, NULL, 0};
Node *root = NULL;
Node *current = NULL;

void initializeFreeBlocks() {
    FreeBlock *tail = NULL;
    for (int idx = 0; idx < TOTAL_BLOCKS; idx++) {
        FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
        if (block == NULL) {
            printf("Memory allocation failed for free blocks.\n");
            exit(1);
        }
        block->blockNumber = idx;
        block->next = NULL;
        block->prev = tail;
        if (freeList.head == NULL) freeList.head = block;
        else tail->next = block;
        tail = block;
        freeList.freeCount++;
    }
    freeList.tail = tail;
}

int getFreeBlock() {
    if (freeList.head == NULL) return -1;
    FreeBlock *block = freeList.head;
    int blockNum = block->blockNumber;
    freeList.head = block->next;
    if (freeList.head != NULL) freeList.head->prev = NULL;
    else freeList.tail = NULL;
    free(block);
    freeList.freeCount--;
    return blockNum;
}

void returnFreeBlock(int blockNum) {
    FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
    if (block == NULL) {
        printf("Could not return free block to list.\n");
        return;
    }
    block->blockNumber = blockNum;
    block->next = NULL;
    block->prev = freeList.tail;
    if (freeList.tail != NULL) freeList.tail->next = block;
    else freeList.head = block;
    freeList.tail = block;
    freeList.freeCount++;
    memset(fileData[blockNum], 0, BLOCK_SIZE);
}

Node *createNode(const char *name, int type, Node *parent) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Memory allocation failed for node.\n");
        exit(1);
    }
    strcpy(node->name, name);
    node->type = type;
    node->parent = parent;
    node->next = node;
    node->prev = node;
    node->childHead = NULL;
    node->allocatedBlocks = NULL;
    node->numBlocks = 0;
    node->fileSize = 0;
    return node;
}

void initializeFileSystem() {
    root = createNode("/", 1, NULL);
    current = root;
    printf("\nVirtual File System initialized successfully.\n");
    printf("Root directory created.\n");
    printf("Disk space available: %d blocks.\n", TOTAL_BLOCKS);
    printf("Type 'help' to see available commands.\n\n");
}

Node *findChild(Node *folder, const char *name) {
    if (folder->childHead == NULL) return NULL;
    Node *temp = folder->childHead;
    do {
        if (strcmp(temp->name, name) == 0) return temp;
        temp = temp->next;
    } while (temp != folder->childHead);
    return NULL;
}

void addChild(Node *parent, Node *child) {
    if (parent->childHead == NULL) {
        parent->childHead = child;
        child->next = child;
        child->prev = child;
    } else {
        Node *first = parent->childHead;
        Node *last = first->prev;
        last->next = child;
        child->prev = last;
        child->next = first;
        first->prev = child;
    }
}

void removeChild(Node *parent, Node *child) {
    if (parent->childHead == NULL) return;
    if (child->next == child) parent->childHead = NULL;
    else {
        child->prev->next = child->next;
        child->next->prev = child->prev;
        if (parent->childHead == child) parent->childHead = child->next;
    }
}

void makeDirectory(const char *dirName) {
    if (dirName == NULL || strlen(dirName) == 0) {
        printf("Invalid directory name.\n");
        return;
    }
    if (findChild(current, dirName) != NULL) {
        printf("Directory '%s' already exists.\n", dirName);
        return;
    }
    Node *newDir = createNode(dirName, 1, current);
    addChild(current, newDir);
    printf("Directory '%s' created inside '%s'.\n", dirName, current->name);
}

void createFile(const char *fileName) {
    if (fileName == NULL || strlen(fileName) == 0) {
        printf("Invalid file name.\n");
        return;
    }
    if (findChild(current, fileName) != NULL) {
        printf("File '%s' already exists.\n", fileName);
        return;
    }
    Node *newFile = createNode(fileName, 2, current);
    addChild(current, newFile);
    printf("File '%s' created inside '%s'.\n", fileName, current->name);
}

void changeDirectory(const char *dirName) {
    if (dirName == NULL) {
        printf("cd: Missing operand.\n");
        return;
    }
    if (strcmp(dirName, "..") == 0) {
        if (current->parent != NULL) current = current->parent;
        return;
    }
    if (strcmp(dirName, "/") == 0) {
        current = root;
        return;
    }
    Node *dir = findChild(current, dirName);
    if (dir == NULL) {
        printf("cd: '%s': Directory not found.\n", dirName);
        return;
    }
    if (dir->type != 1) {
        printf("cd: '%s': Not a directory.\n", dirName);
        return;
    }
    current = dir;
}

void writeFile(const char *fileName) {
    Node *file = findChild(current, fileName);
    if (file == NULL) {
        printf("File not found. Use 'create %s' first.\n", fileName);
        return;
    }
    if (file->type != 2) {
        printf("'%s' is a directory.\n", fileName);
        return;
    }

    printf("Start typing file content (Press Ctrl+D then Enter to save):\n");

    if (file->numBlocks > 0) {
        for (int i = 0; i < file->numBlocks; i++) returnFreeBlock(file->allocatedBlocks[i]);
        free(file->allocatedBlocks);
        file->allocatedBlocks = NULL;
        file->numBlocks = 0;
        file->fileSize = 0;
    }

    char buffer[10240] = "";
    char line[1024];
    while (fgets(line, sizeof(line), stdin) != NULL) strcat(buffer, line);

    int bytes = strlen(buffer);
    if (bytes == 0) {
        printf("File '%s' is empty.\n", fileName);
        return;
    }

    int blocks = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocks > freeList.freeCount) {
        printf("Not enough space. Needed %d blocks, available %d.\n", blocks, freeList.freeCount);
        return;
    }

    file->allocatedBlocks = (int *)malloc(blocks * sizeof(int));
    file->numBlocks = blocks;
    file->fileSize = bytes;

    char *ptr = buffer;
    int remaining = bytes;

    for (int i = 0; i < blocks; i++) {
        int blockNum = getFreeBlock();
        if (blockNum == -1) break;
        file->allocatedBlocks[i] = blockNum;
        int len = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy(fileData[blockNum], ptr, len);
        ptr += len;
        remaining -= len;
    }

    printf("File '%s' saved successfully (%d bytes).\n", fileName, file->fileSize);
}

void readFile(const char *fileName) {
    Node *file = findChild(current, fileName);
    if (file == NULL) {
        printf("'%s' not found.\n", fileName);
        return;
    }
    if (file->type != 2) {
        printf("'%s' is a directory.\n", fileName);
        return;
    }
    if (file->fileSize == 0) {
        printf("File '%s' is empty.\n", fileName);
        return;
    }

    printf("Contents of '%s':\n", fileName);
    char *buffer = (char *)malloc(file->fileSize + 1);
    char *ptr = buffer;
    int remaining = file->fileSize;
    for (int i = 0; i < file->numBlocks; i++) {
        int block = file->allocatedBlocks[i];
        int len = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy(ptr, fileData[block], len);
        ptr += len;
        remaining -= len;
    }
    buffer[file->fileSize] = '\0';
    printf("%s\n", buffer);
    free(buffer);
}

void deleteFile(const char *fileName) {
    Node *file = findChild(current, fileName);
    if (file == NULL) {
        printf("File '%s' not found.\n", fileName);
        return;
    }
    if (file->type != 2) {
        printf("'%s' is a directory.\n", fileName);
        return;
    }
    if (file->numBlocks > 0) {
        for (int i = 0; i < file->numBlocks; i++) returnFreeBlock(file->allocatedBlocks[i]);
        free(file->allocatedBlocks);
    }
    removeChild(current, file);
    free(file);
    printf("File '%s' deleted successfully.\n", fileName);
}

void removeDirectory(const char *dirName) {
    Node *dir = findChild(current, dirName);
    if (dir == NULL) {
        printf("Directory '%s' not found.\n", dirName);
        return;
    }
    if (dir->type != 1) {
        printf("'%s' is not a directory.\n", dirName);
        return;
    }
    if (dir->childHead != NULL) {
        printf("Directory '%s' is not empty.\n", dirName);
        return;
    }
    removeChild(current, dir);
    free(dir);
    printf("Directory '%s' removed successfully.\n", dirName);
}

void listItems() {
    printf("Listing items in '%s':\n", current->name);
    if (current->childHead == NULL) {
        printf("   (empty)\n");
        return;
    }
    Node *temp = current->childHead;
    do {
        if (temp->type == 1) printf("   [DIR]  %s/\n", temp->name);
        else printf("   [FILE] %s\n", temp->name);
        temp = temp->next;
    } while (temp != current->childHead);
}

void getDynamicPrompt(char *prompt) {
    if (current == root) {
        sprintf(prompt, "/ > ");
        return;
    }
    char path[1024] = "";
    Node *dir = current;
    while (dir != root) {
        char temp[1024];
        sprintf(temp, "/%s%s", dir->name, path);
        strcpy(path, temp);
        dir = dir->parent;
    }
    sprintf(prompt, "%s > ", path);
}

void printWorkingDirectory() {
    if (current == root) {
        printf("/\n");
        return;
    }
    char path[1024] = "";
    Node *dir = current;
    while (dir != root) {
        char temp[1024];
        sprintf(temp, "/%s%s", dir->name, path);
        strcpy(path, temp);
        dir = dir->parent;
    }
    printf("%s\n", path);
}

void showDiskInfo() {
    printf("Disk Information:\n");
    printf("   Total Blocks: %d\n", TOTAL_BLOCKS);
    printf("   Free Blocks:  %d\n", freeList.freeCount);
    printf("   Used Blocks:  %d\n", TOTAL_BLOCKS - freeList.freeCount);
    double usage = ((TOTAL_BLOCKS - freeList.freeCount) / (double)TOTAL_BLOCKS) * 100.0;
    printf("   Disk Usage:   %.2f%%\n", usage);
}

void cleanup(Node *dir) {
    if (dir == NULL) return;
    if (dir->childHead != NULL) {
        Node *temp = dir->childHead;
        Node *head = dir->childHead;
        Node *next;
        do {
            next = temp->next;
            cleanup(temp);
            temp = next;
        } while (temp != NULL && temp != head);
    }
    if (dir->type == 2 && dir->allocatedBlocks != NULL) {
        free(dir->allocatedBlocks);
    }
    free(dir);
}

void cleanupFreeList() {
    FreeBlock *temp = freeList.head;
    while (temp != NULL) {
        FreeBlock *next = temp->next;
        free(temp);
        temp = next;
    }
}

void printHelp() {
    printf("\nAvailable Commands:\n");
    printf("   mkdir <dir>   - Create a directory\n");
    printf("   rmdir <dir>   - Remove an empty directory\n");
    printf("   create <file> - Create a new file\n");
    printf("   write <file>  - Write content to a file\n");
    printf("   read <file>   - Display file content\n");
    printf("   delete <file> - Delete a file\n");
    printf("   ls            - List directory contents\n");
    printf("   cd <dir>      - Change directory\n");
    printf("   pwd           - Show current directory\n");
    printf("   df            - Show disk usage info\n");
    printf("   help          - Display this help menu\n");
    printf("   exit          - Exit the system\n\n");
}

int main() {
    char input[INPUT_SIZE], *cmd, *arg, prompt[1024];
    initializeFreeBlocks();
    initializeFileSystem();

    while (1) {
        getDynamicPrompt(prompt);
        printf("%s", prompt);
        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            printf("\nExiting\n");
            break;
        }

        cmd = strtok(input, " \n");
        if (cmd == NULL) continue;
        arg = strtok(NULL, " \n");

        if (strcmp(cmd, "exit") == 0) break;
        else if (strcmp(cmd, "help") == 0) printHelp();
        else if (strcmp(cmd, "ls") == 0) listItems();
        else if (strcmp(cmd, "mkdir") == 0) makeDirectory(arg);
        else if (strcmp(cmd, "create") == 0) createFile(arg);
        else if (strcmp(cmd, "write") == 0) writeFile(arg);
        else if (strcmp(cmd, "read") == 0) readFile(arg);
        else if (strcmp(cmd, "delete") == 0) deleteFile(arg);
        else if (strcmp(cmd, "rmdir") == 0) removeDirectory(arg);
        else if (strcmp(cmd, "cd") == 0) changeDirectory(arg);
        else if (strcmp(cmd, "df") == 0) showDiskInfo();
        else if (strcmp(cmd, "pwd") == 0) printWorkingDirectory();
        else printf("Unknown command: '%s'. Type 'help' for a list of commands.\n", cmd);
    }

    printf("\nReleasing resources and shutting down\n");
    cleanup(root);
    cleanupFreeList();
    printf("All memory freed.\n");
    return 0;
}
