/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;
};
*/

class Solution {
public:
    Node* flatten(Node* head) {
            Node *curr = head;
            while (curr != NULL){
                if (curr->child == NULL){
                    curr = curr->next;
                    continue;
                }

                Node *nextNode = curr->next;

                Node *childNode = curr->child;

                curr->next =  childNode;
                childNode->prev = curr;
                Node *temp = childNode;

                while(temp->next != NULL)
                    temp = temp->next;
                temp->next = nextNode;
                if(nextNode != NULL)
                    nextNode->prev = temp;
                curr->child = NULL;

                curr = curr->next;
                
            }

            return head;
    }
};