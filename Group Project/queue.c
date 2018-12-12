#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>


typedef struct __node_t {
    int value;
    struct __node_t * next;
} node_t;

typedef struct __queue_t {
    node_t * head;
    node_t * tail;
} queue_t;

void Queue_Init(queue_t * q) {
    node_t * tmp = malloc(sizeof(node_t));
    tmp->next = NULL;
    q->head = q->tail = tmp;
}

void Queue_Enqueue(queue_t * q, int value) {
    node_t * tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->value = value;
    tmp->next = NULL;
    q->tail->next = tmp;
    q->tail = tmp;
}

int Queue_Dequeue(queue_t * q) {
    node_t * tmp = q->head;
    node_t * newHead = tmp->next;
    if (newHead == NULL) {
        return -1; // queue was empty
    }
    q->head = newHead;
    free(tmp);
    return 0;
}

void *test(void *list){
    printf("thread \n");
    queue_t *q = (queue_t *) list;
    Queue_Enqueue(q, 1);
    Queue_Enqueue(q, 3);
    Queue_Enqueue(q, 4);
    Queue_Enqueue(q, 2);
    Queue_Dequeue(q);
    printf("thread done\n");
    return NULL;

}

void print_Queue(queue_t *list){
    node_t *current = list->head;
    while(current){
        printf("%d\n",current->value);
        current = current->next;
    }
}


int main(int argc, char **argv){
    printf("start main\n");

    queue_t q;
    
    Queue_Init(&q);

    pthread_t p1, p2;

    pthread_create(&p1, NULL, test, (void *) &q);
    pthread_create(&p2, NULL, test, (void *) &q);

    (void)pthread_join(p1, NULL);
    (void)pthread_join(p2, NULL);

    printf("printing queue\n");
    print_Queue(&q);

    printf("end main program");



    return 0;
}