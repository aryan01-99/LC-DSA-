int findMin(int* nums, int numsSize) {

    int left = 0;
    int right = numsSize - 1;

    while (left < right) {

        int mid = left + (right - left) / 2;

        if (nums[mid] < nums[right]) {
            // Minimum is at mid or on the left side
            right = mid;
        }
        else if (nums[mid] > nums[right]) {
            // Minimum is on the right side
            left = mid + 1;
        }
        else {
            // nums[mid] == nums[right]
            // We cannot determine the side,
            // so safely remove the duplicate right element
            right--;
        }
    }

    return nums[left];
}