#include <stdlib.h>

typedef struct Node {
    int key;
    int value;
    int freq;

    struct Node *prev;
    struct Node *next;
    struct Node *hnext;
} Node;

typedef struct {
    int capacity;
    int size;
    int minFreq;

    int tableSize;
    Node **table;

    Node **head;
    Node **tail;
    int freqCapacity;
} LFUCache;


/* Hash function */
static int hashKey(int key, int size) {
    unsigned int x = (unsigned int)key;

    x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x846ca68b;
    x ^= x >> 16;

    return x % size;
}


/* Find node */
static Node* findNode(LFUCache* obj, int key) {
    int index = hashKey(key, obj->tableSize);

    Node *cur = obj->table[index];

    while (cur != NULL) {
        if (cur->key == key)
            return cur;

        cur = cur->hnext;
    }

    return NULL;
}


/* Insert into hash table */
static void hashInsert(LFUCache* obj, Node *node) {
    int index = hashKey(node->key, obj->tableSize);

    node->hnext = obj->table[index];
    obj->table[index] = node;
}


/* Remove from hash table */
static void hashRemove(LFUCache* obj, Node *node) {
    int index = hashKey(node->key, obj->tableSize);

    Node *cur = obj->table[index];
    Node *prev = NULL;

    while (cur != NULL) {

        if (cur == node) {

            if (prev != NULL)
                prev->hnext = cur->hnext;
            else
                obj->table[index] = cur->hnext;

            return;
        }

        prev = cur;
        cur = cur->hnext;
    }
}


/*
   IMPORTANT:
   Frequency can become larger than capacity.
   So dynamically increase head/tail arrays.
*/
static void ensureFreqCapacity(LFUCache* obj, int freq) {

    if (freq < obj->freqCapacity)
        return;

    int oldCapacity = obj->freqCapacity;
    int newCapacity = oldCapacity;

    while (newCapacity <= freq)
        newCapacity *= 2;

    obj->head = (Node**)realloc(
        obj->head,
        newCapacity * sizeof(Node*)
    );

    obj->tail = (Node**)realloc(
        obj->tail,
        newCapacity * sizeof(Node*)
    );

    for (int i = oldCapacity; i < newCapacity; i++) {
        obj->head[i] = NULL;
        obj->tail[i] = NULL;
    }

    obj->freqCapacity = newCapacity;
}


/* Remove node from frequency list */
static void removeFromList(LFUCache* obj, Node *node) {

    int f = node->freq;

    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        obj->head[f] = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        obj->tail[f] = node->prev;

    node->prev = NULL;
    node->next = NULL;
}


/* Insert node at front */
static void insertFront(LFUCache* obj, Node *node) {

    int f = node->freq;

    /* Make sure array is large enough */
    ensureFreqCapacity(obj, f);

    node->prev = NULL;
    node->next = obj->head[f];

    if (obj->head[f] != NULL)
        obj->head[f]->prev = node;
    else
        obj->tail[f] = node;

    obj->head[f] = node;
}


/* Increase frequency */
static void increaseFreq(LFUCache* obj, Node *node) {

    int oldFreq = node->freq;

    removeFromList(obj, node);

    if (obj->minFreq == oldFreq &&
        obj->head[oldFreq] == NULL) {

        obj->minFreq++;
    }

    node->freq++;

    /* Ensure new frequency exists */
    ensureFreqCapacity(obj, node->freq);

    insertFront(obj, node);
}


/* Create */
LFUCache* lFUCacheCreate(int capacity) {

    LFUCache *obj =
        (LFUCache*)malloc(sizeof(LFUCache));

    obj->capacity = capacity;
    obj->size = 0;
    obj->minFreq = 0;

    obj->tableSize = 100003;

    obj->table =
        (Node**)calloc(
            obj->tableSize,
            sizeof(Node*)
        );

    /*
       Start with a small frequency array.
       It will automatically grow.
    */
    obj->freqCapacity = 16;

    while (obj->freqCapacity <= capacity)
        obj->freqCapacity *= 2;

    obj->head =
        (Node**)calloc(
            obj->freqCapacity,
            sizeof(Node*)
        );

    obj->tail =
        (Node**)calloc(
            obj->freqCapacity,
            sizeof(Node*)
        );

    return obj;
}


/* Get */
int lFUCacheGet(LFUCache* obj, int key) {

    Node *node = findNode(obj, key);

    if (node == NULL)
        return -1;

    increaseFreq(obj, node);

    return node->value;
}


/* Put */
void lFUCachePut(
    LFUCache* obj,
    int key,
    int value
) {

    if (obj->capacity == 0)
        return;

    /* Key already exists */
    Node *node = findNode(obj, key);

    if (node != NULL) {

        node->value = value;

        increaseFreq(obj, node);

        return;
    }


    /* Cache is full */
    if (obj->size == obj->capacity) {

        Node *victim =
            obj->tail[obj->minFreq];

        removeFromList(obj, victim);

        hashRemove(obj, victim);

        free(victim);

        obj->size--;
    }


    /* Create new node */
    node =
        (Node*)malloc(sizeof(Node));

    node->key = key;
    node->value = value;
    node->freq = 1;

    node->prev = NULL;
    node->next = NULL;
    node->hnext = NULL;


    hashInsert(obj, node);

    insertFront(obj, node);

    obj->size++;

    obj->minFreq = 1;
}


/* Free */
void lFUCacheFree(LFUCache* obj) {

    if (obj == NULL)
        return;

    for (int i = 0; i < obj->tableSize; i++) {

        Node *cur = obj->table[i];

        while (cur != NULL) {

            Node *next = cur->hnext;

            free(cur);

            cur = next;
        }
    }

    free(obj->table);
    free(obj->head);
    free(obj->tail);

    free(obj);
}