/**
 * Definition for a Node.
 * struct Node {
 *     int val;
 *     struct Node *next;
 *     struct Node *random;
 * };
 */

struct Node* copyRandomList(struct Node* head) {
    if (head == NULL)
        return NULL;

    struct Node *curr = head;

   
    while (curr != NULL) {
        struct Node *copy = malloc(sizeof(struct Node));

        copy->val = curr->val;
        copy->next = curr->next;
        copy->random = NULL;

        curr->next = copy;
        curr = copy->next;
    }

    curr = head;

    while (curr != NULL) {
        if (curr->random != NULL)
            curr->next->random = curr->random->next;

        curr = curr->next->next;
    }

    
    curr = head;

    struct Node *newHead = head->next;

    while (curr != NULL) {
        struct Node *copy = curr->next;

        curr->next = copy->next;

        if (copy->next != NULL)
            copy->next = copy->next->next;

        curr = curr->next;
    }

    return newHead;   
}