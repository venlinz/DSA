#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_VECTOR_CAPACITY 8

typedef struct {
    int32_t * data;
    size_t length;
    size_t capacity;
} Vector;

void vec_print_elements(Vector a) {
    printf("length of vector: %zu\n", a.length);
    for (size_t i = 0; i < a.length; i++) {
        printf("  %zuth: %i\n", i + 1, a.data[i]);
    }
}

void vec_remove_at(Vector *a, size_t pos) {
    assert(pos < a->length && "Index out of bounds");
    if (pos == a->length - 1) {
        a->length--;
        return;
    }
    memmove(a->data + pos, a->data + pos + 1, a->length - pos);
    a->length--;
}

uint32_t vec_get(Vector a, size_t pos) {
    assert(pos < a.length && "Index out of bounds");
    return a.data[pos];
}

int32_t vec_append(Vector *a, int element) {
    if (a->capacity <= 0 || a->data == NULL) {
        return -1;
    }
    if (a->length < a->capacity) {
        a->data[a->length] = element;
        a->length++;
    } else {
        size_t new_capacity = a->capacity * 2;
        assert(a->capacity < new_capacity && "Overflow occurred");
        a->capacity = new_capacity;
        int32_t * new_data = realloc(a->data, new_capacity * sizeof(int32_t));
        if (new_data == NULL) {
            fprintf(stderr, "mem realloc failed\n");
            exit(1);
        }
        a->data = new_data;
        a->data[a->length] = element;
        a->length++;
    }
    return 0;
}

void vec_clear(Vector *a) {
    a->length = 0;
}

void vec_free(Vector *a) {
    a->length = 0;
    a->capacity = 0;
    free(a->data);
    a->data = NULL;
}

Vector make_vector() {
    size_t capacity = DEFAULT_VECTOR_CAPACITY * sizeof(int32_t);
    if (capacity == 0) {
        return (Vector) {.data = NULL, .length = 0, .capacity = 0};
    }
    return (Vector) {
        .data = malloc(capacity),
        .length = 0,
        .capacity = DEFAULT_VECTOR_CAPACITY,
    };
}

int main(void) {
    Vector a = make_vector();
    for (size_t i = 0; i < 100000000; ++i) {
        vec_append(&a, i);
    }
    printf("element at postition 8: %i\n", vec_get(a, 8));
    for (size_t i = 0; i < 5000; ++i) {
        vec_remove_at(&a, 8);
    }
    printf("element at postition 8 after removing element at position 8: %i\n", vec_get(a, 8));
    vec_remove_at(&a, 0);
    vec_free(&a);
    return EXIT_SUCCESS;
}
