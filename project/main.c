#include <stdio.h>
#include <assert.h>
#include "common.h"

int main() {
    vector_t temperatures;
    assert(create_vector(&temperatures) == SUCCESS);
    FILE *input = fopen("samples/sample1.txt", "r");
    if (!input) {
        return NULLPTR_ERROR;
    }
    assert(read_vector(&temperatures, input) == SUCCESS);
    fclose(input);
    time_diff_t result;
    find_max_diff(temperatures.array, temperatures.size, &result);
    printf("%d", result.time);
    delete_vector(&temperatures);
    return 0;
}
