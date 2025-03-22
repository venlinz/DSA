#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_VECTOR_CAPACITY 8

typedef struct {
    void **data;
    size_t length;
    size_t capacity;
} Vector_t;

void * vec_remove_at(Vector_t *a, size_t pos) {
    assert(pos < a->length && "Index out of bounds");
    if (pos == a->length - 1) {
        a->length--;
        return a->data[a->length + 1];
    }
    void * non_pointed_ref = a->data[pos];
    memmove(a->data + pos, a->data + pos + 1, (a->length - pos - 1) * (sizeof(void *)));
    a->length--;
    return non_pointed_ref;
}

void * vec_get(Vector_t a, size_t pos) {
    assert(pos < a.length && "Index out of bounds");
    return a.data[pos];
}

uint8_t vec_append(Vector_t *a, void * element) {
    if (a->capacity <= 0 || a->data == NULL) {
        a->capacity = DEFAULT_VECTOR_CAPACITY;
        a->data = malloc(DEFAULT_VECTOR_CAPACITY * sizeof(void *));
    }
    if (a->length < a->capacity) {
        a->data[a->length] = element;
        a->length++;
    } else {
        size_t new_capacity = a->capacity * 2;
        assert(a->capacity < new_capacity && "Overflow occurred");
        a->capacity = new_capacity;
        void *new_data = realloc(a->data, new_capacity * sizeof(void *));
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

void vec_clear(Vector_t *a) {
    a->length = 0;
}

void vec_free(Vector_t *a) {
    for (size_t i = 0; i < a->length; ++i) {
        free(a->data[i]);
    }
    a->length = 0;
    a->capacity = 0;
    free(a->data);
    a->data = NULL;
}

Vector_t make_vector() {
    size_t capacity = DEFAULT_VECTOR_CAPACITY * sizeof(void *);
    return (Vector_t) {
        .data = malloc(capacity),
        .length = 0,
        .capacity = DEFAULT_VECTOR_CAPACITY,
    };
}

int main(void) {
    Vector_t a = make_vector();
    typedef struct {
        int a;
        int b;
    } MyStruct;
    for (size_t i = 0; i < 24; ++i) {
        MyStruct *elem = malloc(sizeof(MyStruct));
        elem->a = i;
        elem->b = i * 2;

        vec_append(&a, elem);
    }
    printf("element at postition 1: %i\n", ((MyStruct *) vec_get(a, 1))->a);
    free(vec_remove_at(&a, 8));
    printf("element at postition 8: %i\n", ((MyStruct *) vec_get(a, 8))->a);

    printf("element at postition 0: %i\n", ((MyStruct *) vec_get(a, 0))->a);
    free(vec_remove_at(&a, 0));
    printf("element at postition 0 after removing element at position 0: a: %i, b: %i\n", ((MyStruct *) vec_get(a, 0))->a, ((MyStruct *) vec_get(a, 0))->b);
    vec_free(&a);
    return EXIT_SUCCESS;
}
