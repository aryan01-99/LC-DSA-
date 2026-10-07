/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

typedef struct {
    int num;
    int freq;
} Pair;

int compare(const void *a, const void *b) {
    Pair *x = (Pair *)a;
    Pair *y = (Pair *)b;

    return y->freq - x->freq;
}

int* topKFrequent(int* nums, int numsSize, int k, int* returnSize) {

    Pair *arr = malloc(numsSize * sizeof(Pair));
    int unique = 0;

    // Count frequency of each number
    for (int i = 0; i < numsSize; i++) {

        int found = 0;

        for (int j = 0; j < unique; j++) {
            if (arr[j].num == nums[i]) {
                arr[j].freq++;
                found = 1;
                break;
            }
        }

        if (!found) {
            arr[unique].num = nums[i];
            arr[unique].freq = 1;
            unique++;
        }
    }

    // Sort by frequency (highest first)
    qsort(arr, unique, sizeof(Pair), compare);

    // Store top k elements
    int *ans = malloc(k * sizeof(int));

    for (int i = 0; i < k; i++) {
        ans[i] = arr[i].num;
    }

    *returnSize = k;

    free(arr);

    return ans;
}