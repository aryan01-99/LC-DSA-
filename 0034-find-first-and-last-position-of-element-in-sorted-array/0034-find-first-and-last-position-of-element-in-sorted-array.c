int findFirst(int* nums, int numsSize, int target) {
    int left = 0;
    int right = numsSize - 1;
    int ans = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            ans = mid;
            right = mid - 1; // Search further left
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return ans;
}

int findLast(int* nums, int numsSize, int target) {
    int left = 0;
    int right = numsSize - 1;
    int ans = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            ans = mid;
            left = mid + 1; // Search further right
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return ans;
}

int* searchRange(int* nums, int numsSize, int target, int* returnSize) {

    int* result = malloc(2 * sizeof(int));

    result[0] = findFirst(nums, numsSize, target);
    result[1] = findLast(nums, numsSize, target);

    *returnSize = 2;

    return result;
}