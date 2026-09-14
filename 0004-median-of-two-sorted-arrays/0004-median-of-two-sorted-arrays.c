#include <limits.h>

double findMedianSortedArrays(int* nums1, int nums1Size,
                              int* nums2, int nums2Size) {

    // Make nums1 the smaller array
    if (nums1Size > nums2Size) {
        int* tempNums = nums1;
        nums1 = nums2;
        nums2 = tempNums;

        int tempSize = nums1Size;
        nums1Size = nums2Size;
        nums2Size = tempSize;
    }

    int m = nums1Size;
    int n = nums2Size;

    int left = 0;
    int right = m;

    while (left <= right) {

        // Partition of nums1
        int partition1 = left + (right - left) / 2;

        // Partition of nums2
        int partition2 = (m + n + 1) / 2 - partition1;

        int left1 = (partition1 == 0)
                    ? INT_MIN
                    : nums1[partition1 - 1];

        int right1 = (partition1 == m)
                     ? INT_MAX
                     : nums1[partition1];

        int left2 = (partition2 == 0)
                    ? INT_MIN
                    : nums2[partition2 - 1];

        int right2 = (partition2 == n)
                     ? INT_MAX
                     : nums2[partition2];

        // Correct partition
        if (left1 <= right2 && left2 <= right1) {

            // Total number of elements is odd
            if ((m + n) % 2 == 1) {
                return (double)(
                    left1 > left2 ? left1 : left2
                );
            }

            // Total number of elements is even
            int maxLeft = left1 > left2 ? left1 : left2;
            int minRight = right1 < right2 ? right1 : right2;

            return ((double)maxLeft + minRight) / 2.0;
        }

        // We took too many elements from nums1
        else if (left1 > right2) {
            right = partition1 - 1;
        }

        // We took too few elements from nums1
        else {
            left = partition1 + 1;
        }
    }

    return 0.0;
}