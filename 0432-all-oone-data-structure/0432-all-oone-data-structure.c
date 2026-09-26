#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10007

typedef struct KeyNode KeyNode;
typedef struct Bucket Bucket;

/* One key */
struct KeyNode {
    char *key;

    KeyNode *prev;
    KeyNode *next;

    Bucket *bucket;

    /* Hash table chain */
    KeyNode *hashNext;
};

/* One frequency bucket */
struct Bucket {
    int count;

    KeyNode *keys;

    Bucket *prev;
    Bucket *next;
};

typedef struct {
    Bucket *head;   // minimum frequency
    Bucket *tail;   // maximum frequency

    KeyNode *hash[HASH_SIZE];
} AllOne;


/* ---------------- HASH FUNCTION ---------------- */

unsigned int getHash(char *key) {
    unsigned long h = 5381;

    while (*key) {
        h = ((h << 5) + h) + (unsigned char)*key;
        key++;
    }

    return h % HASH_SIZE;
}


/* ---------------- FIND KEY ---------------- */

KeyNode *findKey(AllOne *obj, char *key) {

    unsigned int index = getHash(key);

    KeyNode *cur = obj->hash[index];

    while (cur != NULL) {

        if (strcmp(cur->key, key) == 0)
            return cur;

        cur = cur->hashNext;
    }

    return NULL;
}


/* ---------------- CREATE BUCKET ---------------- */

Bucket *createBucket(int count) {

    Bucket *b = malloc(sizeof(Bucket));

    b->count = count;
    b->keys = NULL;
    b->prev = NULL;
    b->next = NULL;

    return b;
}


/* ---------------- ADD KEY TO BUCKET ---------------- */

void addKey(Bucket *b, KeyNode *node) {

    node->prev = NULL;
    node->next = b->keys;

    if (b->keys != NULL)
        b->keys->prev = node;

    b->keys = node;

    node->bucket = b;
}


/* ---------------- REMOVE KEY FROM BUCKET ---------------- */

void removeKey(Bucket *b, KeyNode *node) {

    if (node->prev)
        node->prev->next = node->next;
    else
        b->keys = node->next;

    if (node->next)
        node->next->prev = node->prev;

    node->prev = NULL;
    node->next = NULL;
}


/* ---------------- REMOVE EMPTY BUCKET ---------------- */

void removeBucket(AllOne *obj, Bucket *b) {

    if (b->prev)
        b->prev->next = b->next;
    else
        obj->head = b->next;

    if (b->next)
        b->next->prev = b->prev;
    else
        obj->tail = b->prev;

    free(b);
}


/* ---------------- CREATE ---------------- */

AllOne *allOneCreate() {

    AllOne *obj = malloc(sizeof(AllOne));

    obj->head = NULL;
    obj->tail = NULL;

    for (int i = 0; i < HASH_SIZE; i++)
        obj->hash[i] = NULL;

    return obj;
}


/* ---------------- INC ---------------- */

void allOneInc(AllOne *obj, char *key) {

    KeyNode *node = findKey(obj, key);

    /* KEY DOES NOT EXIST */
    if (node == NULL) {

        node = malloc(sizeof(KeyNode));

        node->key = malloc(strlen(key) + 1);
        strcpy(node->key, key);

        node->prev = NULL;
        node->next = NULL;
        node->bucket = NULL;

        /* Add to hash table */

        unsigned int index = getHash(key);

        node->hashNext = obj->hash[index];
        obj->hash[index] = node;


        /* Need bucket 1 */

        if (obj->head == NULL || obj->head->count != 1) {

            Bucket *b = createBucket(1);

            b->next = obj->head;

            if (obj->head)
                obj->head->prev = b;
            else
                obj->tail = b;

            obj->head = b;
        }

        addKey(obj->head, node);

        return;
    }


    /* KEY ALREADY EXISTS */

    Bucket *old = node->bucket;

    Bucket *nextBucket;


    /* Need count + 1 bucket */

    if (old->next == NULL ||
        old->next->count != old->count + 1) {

        nextBucket = createBucket(old->count + 1);

        nextBucket->prev = old;
        nextBucket->next = old->next;

        if (old->next)
            old->next->prev = nextBucket;
        else
            obj->tail = nextBucket;

        old->next = nextBucket;

    }
    else {

        nextBucket = old->next;
    }


    removeKey(old, node);

    addKey(nextBucket, node);


    /* Old bucket empty */

    if (old->keys == NULL)
        removeBucket(obj, old);
}


/* ---------------- DEC ---------------- */

void allOneDec(AllOne *obj, char *key) {

    KeyNode *node = findKey(obj, key);

    if (node == NULL)
        return;


    Bucket *old = node->bucket;


    /* Frequency becomes ZERO */

    if (old->count == 1) {

        removeKey(old, node);


        /* Remove from hash table */

        unsigned int index = getHash(key);

        KeyNode *cur = obj->hash[index];
        KeyNode *prev = NULL;

        while (cur != NULL) {

            if (cur == node) {

                if (prev)
                    prev->hashNext = cur->hashNext;
                else
                    obj->hash[index] = cur->hashNext;

                break;
            }

            prev = cur;
            cur = cur->hashNext;
        }


        free(node->key);
        free(node);


        if (old->keys == NULL)
            removeBucket(obj, old);

        return;
    }


    /* Need count - 1 bucket */

    Bucket *prevBucket;

    if (old->prev == NULL ||
        old->prev->count != old->count - 1) {

        prevBucket = createBucket(old->count - 1);

        prevBucket->next = old;
        prevBucket->prev = old->prev;

        if (old->prev)
            old->prev->next = prevBucket;
        else
            obj->head = prevBucket;

        old->prev = prevBucket;

    }
    else {

        prevBucket = old->prev;
    }


    removeKey(old, node);

    addKey(prevBucket, node);


    if (old->keys == NULL)
        removeBucket(obj, old);
}


/* ---------------- GET MAX ---------------- */

char *allOneGetMaxKey(AllOne *obj) {

    if (obj->tail == NULL)
        return "";

    return obj->tail->keys->key;
}


/* ---------------- GET MIN ---------------- */

char *allOneGetMinKey(AllOne *obj) {

    if (obj->head == NULL)
        return "";

    return obj->head->keys->key;
}


/* ---------------- FREE ---------------- */

void allOneFree(AllOne *obj) {

    Bucket *b = obj->head;

    while (b) {

        Bucket *nextBucket = b->next;

        KeyNode *node = b->keys;

        while (node) {

            KeyNode *nextNode = node->next;

            free(node->key);
            free(node);

            node = nextNode;
        }

        free(b);

        b = nextBucket;
    }

    free(obj);
}
/**
 * Your AllOne struct will be instantiated and called as such:
 * AllOne* obj = allOneCreate();
 * allOneInc(obj, key);
 
 * allOneDec(obj, key);
 
 * char* param_3 = allOneGetMaxKey(obj);
 
 * char* param_4 = allOneGetMinKey(obj);
 
 * allOneFree(obj);
*/