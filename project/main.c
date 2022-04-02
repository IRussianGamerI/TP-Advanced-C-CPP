#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[]) {
    int opt;
    char *opts = "i:o:h";
    char *input = NULL;
    char *output = NULL;

    while ((opt = getopt(argc, argv, opts)) != -1) {
        switch (opt) {
            case 'i':
                input = calloc((strlen(optarg) + 1), sizeof(char));
                snprintf(input, strlen(optarg) + 1, "%s", optarg);
                break;
            case 'o':
                output = calloc((strlen(optarg) + 1), sizeof(char));
                snprintf(output, strlen(optarg) + 1, "%s", optarg);
                break;
            case 'h':
                printf("Usage: %s [-h|-i|-o] <args>\n", argv[0]);
                break;
            default:
                break;
        }
    }
    vector_t v;
    if (create_vector(&v) != SUCCESS) {
        printf("Vector creation failed\n");
        return 1;
    }
    if (!input) {
        if (!read_vector(&v, stdin)) {
            printf("Vector reading failed\n");
            return 1;
        }
    } else {
        FILE *file = fopen(input, "r");

        if (!file) {
            printf("File opening failed\n");
            return 1;
        }

        if (read_vector(&v, file) != SUCCESS) {
            printf("Vector reading failed\n");
            return 1;
        }

        if (fclose(file)) {
            printf("File closing failed\n");
            return 1;
        }
    }
    size_t calls_count = 5;

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    time_diff_t result;
    for (size_t i = 0; i < calls_count; ++i) {
        if (find_max_diff(v.array, v.size, &result) != SUCCESS) {
            printf("Error occurred during call No. %zu", i + 1);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    size_t time = (1000000000 * (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec)) / 1000;

    if (!output) {
        printf("Result : %d \n", result.time);
        printf("Time : %zu us\n", time / calls_count);
    } else {
        FILE *file = fopen(output, "w");

        if (!file) {
            printf("Opening file failed\n");
            return 1;
        }

        fprintf(file, "Result : %d \n", result.time);
        fprintf(file, "Time : %zu us\n\n", time / calls_count);

        if (fclose(file)) {
            printf("File closing failed\n");
            return 1;
        }
    }

    if (delete_vector(&v) != SUCCESS) {
        printf("Vector deleting failed\n");
        return 1;
    }
    if (input) {
        free(input);
    }
    if (output) {
        free(output);
    }
    return 0;
}
