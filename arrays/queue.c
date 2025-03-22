#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node_t {
    struct Node_t *next;
    void *data;
} Node_t ;

typedef struct Queue_t {
    Node_t *head;
    Node_t *tail;
    size_t size;
} Queue_t;

Queue_t new_queue(void *data) {
    Node_t * node = malloc(sizeof(Node_t));
    node->data = data;
    node->next = NULL;
    Queue_t queue = {
        .head = node,
        .tail = node,
        .size = 1,
    };
    return queue;
}

void * peek(Queue_t *queue) {
    if (queue->head == NULL) {
        fprintf(stderr, "ERROR: head is empty: peek()\n");
        exit(EXIT_FAILURE);
    }
    return queue->head->data;
}

void enqueue(Queue_t *queue, void *data) {
    Node_t *node = malloc(sizeof(Node_t));
    node->data = data;
    ++queue->size;
    node->next = NULL;
    queue->tail->next = node;
    queue->tail = node;
}

void * dequeue(Queue_t *queue) {
    if (queue->head == NULL) {
        fprintf(stderr, "ERROR: head is empty: dequeue()\n");
        exit(EXIT_FAILURE);
    }
    Node_t *old_head = queue->head;
    void *data = old_head->data;
    queue->head = queue->head->next;
    free(old_head);
    return data;
}

int main(void) {
    int *a = malloc(sizeof(int));
    *a = 100;
    Queue_t queue = new_queue(a);
    printf("peek: %i\n", *(int *)peek(&queue));

    int *b = malloc(sizeof(int));
    *b = 200;
    enqueue(&queue, b);
    int *c = malloc(sizeof(int));
    *c = 300;
    enqueue(&queue, c);

    int *d = dequeue(&queue);
    printf("dequeued: %i\n", *d);
    int *d1 = dequeue(&queue);
    printf("dequeued: %i\n", *d1);
    printf("peek: %i\n", *(int *)peek(&queue));
    int *d2 = dequeue(&queue);
    printf("dequeued: %i\n", *d2);

    free(d);
    free(d1);
    free(d2);

    return EXIT_SUCCESS;
}
