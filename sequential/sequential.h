#ifndef SEQUENTIAL_SEQUENTIAL_H_
#define SEQUENTIAL_SEQUENTIAL_H_

enum ERRORS {
    WRONG_PARAMS = -1,
    NULLPTR = -2
};

int find_max_diff_seq(int *array, int len);

int find_max_diff_on_section(int *array, int begin, int end);

#endif  // SEQUENTIAL_SEQUENTIAL_H_
