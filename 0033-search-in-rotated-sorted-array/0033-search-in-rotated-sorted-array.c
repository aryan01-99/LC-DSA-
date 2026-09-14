int search(int* nums, int numsSize, int target) {

    int left = 0;
    int right = numsSize - 1;

    while (left <= right) {

        int mid = left + (right - left) / 2;

        // Target found
        if (nums[mid] == target) {
            return mid;
        }

        // Left half is sorted
        if (nums[left] <= nums[mid]) {

            // Target lies in the left sorted half
            if (nums[left] <= target && target < nums[mid]) {
                right = mid - 1;
            }
            // Target lies in the right half
            else {
                left = mid + 1;
            }
        }

        // Right half is sorted
        else {

            // Target lies in the right sorted half
            if (nums[mid] < target && target <= nums[right]) {
                left = mid + 1;
            }
            // Target lies in the left half
            else {
                right = mid - 1;
            }
        }
    }

    // Target not found
    return -1;
}