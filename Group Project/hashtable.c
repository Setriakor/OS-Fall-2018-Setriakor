#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#define BUCKETS (1001)

// basic node structure
typedef struct __node_t
{
    int key;
    struct __node_t *next;
} node_t;

// basic list structure (one used per list)
typedef struct __list_t
{
    node_t *head;
} list_t;

void List_Init(list_t *L)
{
    L->head = NULL;
}

int List_Insert(list_t *L, int key)
{
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL)
    {
        perror("malloc");
        return -1; // fail
    }
    new->key = key;
    new->next = L->head;
    L->head = new;
    return 0; // success
}

//My list delete method
int List_Delete(list_t *L, int key)
{
    if (L->head == NULL)
    {
        return -1;
    }
    if (L->head->key == key)
    {
        L->head = NULL;
        return 0;
    }
    else
    {
        node_t *curr = L->head;
        while (curr)
        {
            if (curr->key == key)
            {
                curr->next = curr->next->next;
                return 0; // success
            }
            curr = curr->next;
        }
        return -1; // failure
    }
}

int List_Lookup(list_t *L, int key)
{
    node_t *curr = L->head;
    while (curr)
    {
        if (curr->key == key)
        {
            return 0; // success
        }
        curr = curr->next;
    }
    return -1; // failure
}

typedef struct __hash_t
{
    list_t lists[BUCKETS];
} hash_t;

void Hash_Init(hash_t *H)
{
    int i;
    for (i = 0; i < BUCKETS; i++)
    {
        List_Init(&H->lists[i]);
    }
}

int Hash_Insert(hash_t *H, int key)
{
    int bucket = key % BUCKETS;
    return List_Insert(&H->lists[bucket], key);
}

//My hashtable delete method
int Hash_Delete(hash_t *H, int key)
{
    int bucket = key % BUCKETS;
    return List_Delete(&H->lists[bucket], key);
}

int Hash_Lookup(hash_t *H, int key)
{
    int bucket = key % BUCKETS;
    return List_Lookup(&H->lists[bucket], key);
}

void *hashTest(void *table)
{
    hash_t *tab;
    tab = (hash_t *)table;
    for (int i = 0; i < 200; i++)
    {
        Hash_Insert(tab, i);
    }
}

void *hashTest1(void *table)
{
    hash_t *tab;
    tab = (hash_t *)table;
    for (int i = 0; i < 200; i++)
    {
        int x = Hash_Delete(tab, i);
        // printf("%d", x);
        // printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    hash_t table;
    hash_t *ptrTable;
    ptrTable = &table;

    Hash_Init(ptrTable);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, hashTest, (void *)ptrTable);
    pthread_create(&t2, NULL, hashTest1, (void *)ptrTable);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    for (int i = 0; i < 200; i++)
    {
        int x = Hash_Lookup(ptrTable, i);
        printf("For Key ");
        printf("%d", i);
        printf(" the result of Lookup is: ");
        printf("%d", x);
        printf("\n");
    }
    return 0;
}
