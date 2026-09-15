long long merge(int* nums, int* temp, int left, int mid, int right) {
    long long count = 0;

    int j = mid + 1;

    // Count reverse pairs
    for (int i = left; i <= mid; i++) {
        while (j <= right && (long long)nums[i] > 2LL * nums[j]) {
            j++;
        }

        count += j - (mid + 1);
    }

    // Merge two sorted halves
    int i = left;
    j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (nums[i] <= nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = nums[i++];
    }

    while (j <= right) {
        temp[k++] = nums[j++];
    }

    for (i = left; i <= right; i++) {
        nums[i] = temp[i];
    }

    return count;
}


long long mergeSort(int* nums, int* temp, int left, int right) {
    if (left >= right) {
        return 0;
    }

    int mid = left + (right - left) / 2;

    long long count = 0;

    // Reverse pairs in left half
    count += mergeSort(nums, temp, left, mid);

    // Reverse pairs in right half
    count += mergeSort(nums, temp, mid + 1, right);

    // Reverse pairs between left and right halves
    count += merge(nums, temp, left, mid, right);

    return count;
}


int reversePairs(int* nums, int numsSize) {
    int* temp = (int*)malloc(numsSize * sizeof(int));

    long long answer = mergeSort(nums, temp, 0, numsSize - 1);

    free(temp);

    return (int)answer;
}