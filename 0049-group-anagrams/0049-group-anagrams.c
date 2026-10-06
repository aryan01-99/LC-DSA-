/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

 #include<stdio.h>
 #include<stdlib.h>
 #include<string.h>

 int compare(const void *a, const void *b){
    return (*(char *)a - *(char *)b);
 }
char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
    char*** result= malloc(strsSize * sizeof(char **));
    char **keys = malloc(strsSize * sizeof(char **));
    *returnColumnSizes = malloc(strsSize * sizeof(int));

    int groups =0;

    for(int i=0; i<strsSize; i++){
        char *key = malloc((strlen(strs[i]) +1) * sizeof(char));

        strcpy(key, strs[i]);

        qsort(key, strlen(key), sizeof(char), compare);

        int found = -1;

        for(int j =0; j< groups; j++){
            if(strcmp(key , keys[j]) == 0){
                found = j;
                break;
            }
        }

        if(found == -1){
            keys[groups] = key;
            result[groups] = malloc(strsSize * sizeof(char*));
            result[groups][0] = strs[i];
            (*returnColumnSizes)[groups] = 1;
            groups++;
        }
        else{
            result[found][(*returnColumnSizes)[found]] = strs[i];
            (*returnColumnSizes)[found]++;

            free(key);
        }


    }

    *returnSize = groups;

    

    for(int i=0; i< groups; i++){
         free(keys[i]);
    }
    free(keys);
    return result;



}