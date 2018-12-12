#include <pthread.h>
#include <stdio.h>

typedef struct __counter_t
{
    int value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *c)
{
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    counter_t *c1 = (counter_t *)c;
    for (int i = 0; i < 10000; i++)
    {
        c1->value++;
    }
    pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c)
{
    pthread_mutex_lock(&c->lock);
    for (int i = 0; i < 10000; i--)
        c->value--;
    pthread_mutex_unlock(&c->lock);
}

int main()
{
    counter_t count;
    counter_t *counts;
    counts = &count;
    init(counts);
    int i;
    pthread_t tid;

    for (i = 0; i <= 3; i++)
    {
        pthread_create(&tid, NULL, increment, (void *)counts);
    }
    ///pthread_exit(NULL);

    printf("%d", counts->value);
    return 0;
}