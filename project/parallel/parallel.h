#ifndef PROJECT_PARALLEL_PARALLEL_H_
#define PROJECT_PARALLEL_PARALLEL_H_

#include "common.h"

#define NUMBER_PIPE_DESC 2
#define MAX_TWO_INTS_LEN 24  // Length of a string like "-MAX_INT -MAX_INT"

int **alloc_pipes(int amount);

err_t find_max_from_pipes(int **pipes, int amount, time_diff_t *result);

err_t find_max_diff_on_section(const int *array, int begin, int end, time_diff_t *result);
#endif  // PROJECT_PARALLEL_PARALLEL_H_
