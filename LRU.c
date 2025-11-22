#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 1009

typedef struct Node
{
    int key;
    char val[100];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct MapEntry
{
    int key;
    Node *node;
    struct MapEntry *next;
} MapEntry;

typedef struct LRUCache
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    MapEntry *map[MAP_SIZE];
} LRUCache;

void memCheck(void *node)
{
    if (!node)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
}

int hashFunc(int key)
{
    return key % MAP_SIZE;
}

MapEntry *mapGet(LRUCache *cache, int key)
{
    MapEntry *cur = cache->map[hashFunc(key)];
    while (cur)
    {
        if (cur->key == key)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

void mapPut(LRUCache *cache, int key, Node *node)
{
    int index = hashFunc(key);
    MapEntry *entry = (MapEntry *)malloc(sizeof(MapEntry));
    memCheck(entry);
    entry->key = key;
    entry->node = node;
    entry->next = cache->map[index];
    cache->map[index] = entry;
}

void mapDelete(LRUCache *cache, int key)
{
    int index = hashFunc(key);
    MapEntry *cur = cache->map[index];
    MapEntry *prev = NULL;

    while (cur)
    {
        if (cur->key == key)
        {
            if (prev)
                prev->next = cur->next;
            else
                cache->map[index] = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void moveToHead(LRUCache *cache, Node *node)
{
    if (cache->head == node)
        return;

    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    if (cache->tail == node)
        cache->tail = node->prev;

    node->prev = NULL;
    node->next = cache->head;

    if (cache->head)
        cache->head->prev = node;
    cache->head = node;

    if (!cache->tail)
        cache->tail = node;
}

void removeTail(LRUCache *cache)
{
    if (!cache->tail)
        return;

    Node *del = cache->tail;
    cache->tail = del->prev;

    if (cache->tail)
        cache->tail->next = NULL;
    else
        cache->head = NULL;

    mapDelete(cache, del->key);
    free(del);
    cache->size--;
}

LRUCache *createCache(int capacity)
{
    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    memCheck(cache);

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;

    for (int i = 0; i < MAP_SIZE; i++)
        cache->map[i] = NULL;

    return cache;
}

char *get(LRUCache *cache, int key)
{
    MapEntry *entry = mapGet(cache, key);
    if (!entry)
        return NULL;
    moveToHead(cache, entry->node);
    return entry->node->val;
}

void put(LRUCache *cache, int key, char *val)
{
    MapEntry *entry = mapGet(cache, key);

    if (entry)
    {
        strcpy(entry->node->val, val);
        moveToHead(cache, entry->node);
        return;
    }

    if (cache->size == cache->capacity)
        removeTail(cache);

    Node *node = (Node *)malloc(sizeof(Node));
    memCheck(node);

    node->key = key;
    strcpy(node->val, val);
    node->prev = node->next = NULL;

    node->next = cache->head;
    if (cache->head)
        cache->head->prev = node;
    cache->head = node;

    if (!cache->tail)
        cache->tail = node;

    mapPut(cache, key, node);
    cache->size++;
}

int main()
{
    LRUCache *cache = NULL;
    char cmd[50];

    while (1)
    {
        scanf("%s", cmd);

        if (!strcmp(cmd, "createCache"))
        {
            int size;
            scanf("%d", &size);
            cache = createCache(size);
        }
        else if (!strcmp(cmd, "put"))
        {
            int key;
            char val[100];
            scanf("%d %s", &key, val);
            put(cache, key, val);
        }
        else if (!strcmp(cmd, "get"))
        {
            int key;
            scanf("%d", &key);
            char *result = get(cache, key);
            if (result)
                printf("%s\n", result);
            else
                printf("NULL\n");
        }
        else if (!strcmp(cmd, "exit"))
        {
            break;
        }
    }
    return 0;
}
