/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

 struct ListNode *mergeTwo(struct ListNode *a, struct ListNode *b){
    struct ListNode dummy;
    struct ListNode *tail= &dummy;
    dummy.next = NULL;

    while( a && b){
        if(a->val < b->val){
            tail->next = a;
            a = a->next;
        } else{
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if(a){
        tail->next = a;
    } else{
        tail->next = b;
    }
    return dummy.next;
 }
struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
      if (listsSize == NULL){
        return NULL;
      }

      int interval = 1;
      while(interval < listsSize){
        for (int i  = 0; i+interval < listsSize; i+=interval*2){
            lists[i] = mergeTwo(lists[i], lists[i+interval]);
        }
        interval*=2;
      }

      return lists[0];
}