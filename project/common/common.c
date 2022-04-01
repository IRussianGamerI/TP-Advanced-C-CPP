#include "common.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

err_t create_vector(vector_t *v) {
    if (!v) {
        return NULLPTR_ERROR;
    }
    v->capacity = 1;
    v->size = 0;
    v->array = calloc(sizeof(int), 1);
    if (!v->array) {
        return ALLOC_ERROR;
    }
    return SUCCESS;
}

err_t push_back(vector_t *v, int element) {
    if (!v || !v->array) {
        return NULLPTR_ERROR;
    }
    if (v->capacity == v->size) {
        int *ptr;
        ptr = realloc(v->array, 2 * v->capacity * sizeof(int));
        if (!ptr) {
            return ALLOC_ERROR;
        }
        v->array = ptr;
        v->capacity = 2 * v->capacity;
    }
    v->array[v->size++] = element;
    return SUCCESS;
}

err_t delete_vector(vector_t* v) {
    if (!v || !v->array) {
        return NULLPTR_ERROR;
    }
    free(v->array);
    v->size = 0;
    v->capacity = 0;
    v->array = NULL;
    return SUCCESS;
}

err_t read_vector(vector_t* v, FILE* in) {
    if (!v || !in) {
        return NULLPTR_ERROR;
    }
    char buf[MAX_INT_LEN];
    fgets(buf, MAX_INT_LEN, in);
    size_t count = strtol(buf, NULL, 10);
    for (size_t i = 0; i < count; ++i) {
        fgets(buf, MAX_INT_LEN, in);
        int number = strtol(buf, NULL, 10);
        if (push_back(v, number) != SUCCESS) {
            return ALLOC_ERROR;
        }
    }
    return SUCCESS;
}
