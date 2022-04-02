#include "common.h"

err_t find_max_diff(const int *array, int size, time_diff_t *result) {
    if (!array || !result) {
        return NULLPTR_ERROR;
    }
    if (size < 2) {
        result->diff = 0;
        result->time = 0;
        return WRONG_PARAMS;
    }

    int max_diff = 0;
    int time = 0;

    err_t flag = NOT_FOUND;
    for (int i = 1; i < size; ++i) {
        if (array[i] - array[i - 1] > max_diff) {
            max_diff = array[i] - array[i - 1];
            time = i;
            flag = SUCCESS;
        }
    }
    result->diff = max_diff;
    result->time = time;
    return flag;
}
