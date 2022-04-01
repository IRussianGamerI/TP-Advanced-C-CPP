#include <stdio.h>

#include "common.h"

int main() {
    vector_t temperatures;
    if (create_vector(&temperatures) != SUCCESS) {
        return ALLOC_ERROR;
    }
    FILE *input = fopen(SAMPLE_FILENAME, "r");
    if (!input) {
        return NULLPTR_ERROR;
    }
    if (read_vector(&temperatures, input) != SUCCESS) {
        return ALLOC_ERROR;
    }
    fclose(input);
    time_diff_t result;
    find_max_diff(temperatures.array, temperatures.size, &result);
    printf("%d", result.time);
    delete_vector(&temperatures);
    return 0;
}
