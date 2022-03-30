#include "sequential.h"

int find_max_diff_seq(int *array, int len) {
    return find_max_diff_on_section(array, 0, len);
}

int find_max_diff_on_section(int *array, int begin, int end) {
    if (left_pos < 0 || right_pos < 2) {
        return WRONG_PARAMS;
    }
    if (!array) {
        return NULLPTR;
    }
    int max_diff = 0;
    int ans = 0;
    for (int i = 1; i < right_pos; ++i) {
        if (array[i] - array[i - 1] > max_diff) {
            max_diff = array[i] - array[i - 1];
            ans = i;
        }
    }
    return ans;  // Returns 0 if no positive diff was found
}