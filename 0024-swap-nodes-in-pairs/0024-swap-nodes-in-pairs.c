/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* swapPairs(struct ListNode* head) {
    if (head == NULL || head->next == NULL)
        return head;
    struct ListNode* first = head;
    struct ListNode* second = head->next;

    first->next = swapPairs(second->next);

    second->next = first;
    return second;
}