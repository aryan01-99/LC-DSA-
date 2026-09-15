#include <stdlib.h>

void mergeSort(int *nums, int *index, int *tempIndex,
               int *count, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSort(nums, index, tempIndex, count, left, mid);
    mergeSort(nums, index, tempIndex, count, mid + 1, right);

    int i = left;
    int j = mid + 1;
    int k = left;

    int smaller = 0;

    while (i <= mid && j <= right)
    {
        if (nums[index[j]] < nums[index[i]])
        {
            tempIndex[k++] = index[j++];
            smaller++;
        }
        else
        {
            count[index[i]] += smaller;
            tempIndex[k++] = index[i++];
        }
    }

    while (i <= mid)
    {
        count[index[i]] += smaller;
        tempIndex[k++] = index[i++];
    }

    while (j <= right)
    {
        tempIndex[k++] = index[j++];
    }

    for (i = left; i <= right; i++)
    {
        index[i] = tempIndex[i];
    }
}

int* countSmaller(int* nums, int numsSize, int* returnSize)
{
    *returnSize = numsSize;

    int *count = (int *)calloc(numsSize, sizeof(int));
    int *index = (int *)malloc(numsSize * sizeof(int));
    int *tempIndex = (int *)malloc(numsSize * sizeof(int));

    for (int i = 0; i < numsSize; i++)
    {
        index[i] = i;
    }

    mergeSort(nums, index, tempIndex, count, 0, numsSize - 1);

    free(index);
    free(tempIndex);

    return count;
}