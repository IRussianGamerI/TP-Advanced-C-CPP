#ifndef PROJECT_COMMON_COMMON_H_
#define PROJECT_COMMON_COMMON_H_

#include <stdio.h>

#define MB 1 << 20  // Megabyte is equal to 2^20 bytes
#define MAX_INT_LEN 12
#define SAMPLE_FILENAME "../project/samples/sample1.txt"

typedef enum ERRORS {
    SUCCESS = 0,
    NOT_FOUND,
    WRONG_PARAMS,
    NULLPTR_ERROR,
    PIPE_ERROR,
    ALLOC_ERROR
} err_t;

typedef struct vector {
    unsigned capacity;
    unsigned size;
    int *array;
} vector_t;

typedef struct time_diff {
    int time;
    int diff;
} time_diff_t;

err_t create_vector(vector_t* v);

err_t push_back(vector_t* v, int element);

err_t delete_vector(vector_t* v);

err_t read_vector(vector_t* v, FILE* in);

err_t find_max_diff(const int *array, int size, time_diff_t *result);

#endif  // PROJECT_COMMON_COMMON_H_
