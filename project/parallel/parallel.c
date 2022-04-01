#include "parallel.h"

#include <malloc.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"

int **alloc_pipes(int amount) {
    if (amount < 1) {
        return NULL;
    }

    int **pipes = malloc(sizeof(int *) * amount);
    for (int i = 0; i < amount; ++i) {
        pipes[i] = malloc(sizeof(int) * NUMBER_PIPE_DESC);
        if (pipe(pipes[i])) {
            for (int j = i; j >= 0; --j) {
                free(pipes[j]);
            }
            free(pipes);
            return NULL;
        }
    }
    return pipes;
}

err_t find_max_diff(const int *array, int size, time_diff_t *result) {
    if (!array || !result) {
        return NULLPTR_ERROR;
    }
    if (size < 2) {
        return WRONG_PARAMS;
    }
    int amount = sysconf(_SC_NPROCESSORS_ONLN) < size / 2 ? sysconf(_SC_NPROCESSORS_ONLN) : size / 2;
    int **pipes = alloc_pipes(amount);
    if (!pipes) {
        return ALLOC_ERROR;
    }

    int section_len = size / amount;
    pid_t *waiters = malloc(sizeof(pid_t) * amount);
    if (!waiters) {
        free(pipes);
        return ALLOC_ERROR;
    }
    for (int i = 0; i < amount; ++i) {
        int pid = fork();
        if (pid == -1) {
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(waiters);
            for (int j = 0; j < amount; ++j) {
                free(pipes[j]);
            }
            free(pipes);
            return PIPE_ERROR;
        } else if (!pid) {
            close(pipes[i][0]);
            time_diff_t local_max;
            find_max_diff_on_section(array, section_len * i, i < amount - 1 ? section_len * (i + 1) + 1 :
                                     size, &local_max);
            write(pipes[i][1], &local_max, sizeof(local_max));
            close(pipes[i][1]);
            exit(EXIT_SUCCESS);
        }
        waiters[i] = pid;
    }

    for (int i = 0; i < amount; ++i) {
        waitpid(waiters[i], NULL, 0);
    }
    free(waiters);
    err_t return_code = find_max_from_pipes(pipes, amount, result);
    for (int i = 0; i < amount; ++i) {
        free(pipes[i]);
    }
    free(pipes);
    return return_code;
}

err_t find_max_from_pipes(int **pipes, int amount, time_diff_t *result) {
    if (!pipes || !result) {
        return NULLPTR_ERROR;
    }
    if (amount < 1) {
        return WRONG_PARAMS;
    }
    int max = 0;
    int time = 0;
    time_diff_t current;
    for (int i = 0; i < amount; ++i) {
        if (!pipes[i]) {
            return NULLPTR_ERROR;
        }
        read(pipes[i][0], &current.time, sizeof(int));
        read(pipes[i][0], &current.diff, sizeof(int));
        if (current.diff > max) {
            max = current.diff;
            time = current.time;
        }
    }
    result->diff = max;
    result->time = time;
    return SUCCESS;
}

err_t find_max_diff_on_section(const int *array, int begin, int end, time_diff_t *result) {
    if (!array || !result) {
        return NULLPTR_ERROR;
    }
    if (end - begin < 2) {
        result->diff = 0;
        result->time = 0;
        return WRONG_PARAMS;
    }

    int max_diff = 0;
    int time = 0;

    err_t flag = NOT_FOUND;
    for (int i = begin + 1; i < end; ++i) {
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
