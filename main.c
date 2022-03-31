#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

int my_random(int max, int min) {
    srand(time(NULL));
    int res = (random() % (max + 1 - min)) + min;
    return res;
}

void init_random(int* arr, int len) {
    for (int i = 0; i < len; ++i) {
        arr[i] = my_random(10000, -10000);
    }
}

int main() {
    int *arr = calloc(sizeof(int), 100 * MB / sizeof(int));
    if (!arr) {
        return 1;
    }
    time_diff_t result;
    find_max_diff(arr, 100 * MB / sizeof(int), &result);
    printf("%d", result.time);
    free(arr);
    return 0;
}
