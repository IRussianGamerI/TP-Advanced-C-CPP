#include "parallel.h"

#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "common.h"

int **alloc_pipes(int amount) {
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

error_t find_max_diff(const int *array, int size, time_diff_t *result) {
    if (!array || !result) {
        return NULLPTR_ERROR;
    }
    if (size < 2) {
        return WRONG_PARAMS;
    }
    int cores = sysconf(_SC_NPROCESSORS_ONLN) < size / 2 ? sysconf(_SC_NPROCESSORS_ONLN) : size / 2;
    int **pipes = alloc_pipes(cores);
    if (!pipes) {
        return ALLOC_ERROR;
    }

    int section_len = size / cores;
    int pid;
    pid_t *waiters = malloc(sizeof(pid_t) * cores);
    if (!waiters) {
        free(pipes);
        return ALLOC_ERROR;
    }
    for (int i = 0; i < cores; ++i) {
        pid = fork();
        char *buf = calloc(sizeof(char), MAX_TWO_INTS_LEN);
        if (pid == -1) {
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(buf);
            free(waiters);
            for (int j = 0; j < cores; ++j) {
                free(pipes[j]);
            }
            free(pipes);
            return PIPE_ERROR;
        } else if (!pid) {
            close(pipes[i][0]);
            time_diff_t local_max;
            find_max_diff_on_section(array, section_len * i, i < cores - 1 ? section_len + 1 : size - section_len * i,
                                     &local_max);
            sprintf(buf, "%d %d", local_max.time, local_max.diff);
            write(pipes[i][1], buf, MAX_TWO_INTS_LEN);
            close(pipes[i][1]);
            exit(EXIT_SUCCESS);
        }
        free(buf);
        waiters[i] = pid;
    }

    for (int i = 0; i < cores; ++i) {
        waitpid(waiters[i], NULL, 0);
    }
    return find_max_from_pipes(pipes, cores, result);
}

error_t find_max_from_pipes(int **pipes, int cores, time_diff_t *result) {
    
}

error_t find_max_diff_on_section(const int *array, int begin, int end, time_diff_t *result) {
    if (!array || !result) {
        return NULLPTR_ERROR;
    }
    if (end - begin < 1) {
        result->diff = 0;
        result->time = 0;
        return WRONG_PARAMS;
    }

    int max_diff = 0;
    int time = 0;

    error_t flag = NOT_FOUND;
    for (int i = begin; i <= end; ++i) {
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
