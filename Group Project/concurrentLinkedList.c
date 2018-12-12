#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

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
    pthread_mutex_t lock;
} list_t;

void List_Init(list_t *L)
{
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t *L, int key)
{
    pthread_mutex_lock(&L->lock);
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL)
    {
        perror("malloc");
        pthread_mutex_unlock(&L->lock);
        return -1; // fail
    }
    new->key = key;
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
    return 0; // success
}

//My list delete method
int List_Delete(list_t *L, int key)
{
    pthread_mutex_lock(&L->lock);
    if (L->head == NULL)
    {
        return -1;
    }
    if (L->head->key == key)
    {
        L->head=NULL;
        return 0;
    }
    else
    {
        node_t *curr = L->head;
        while (curr)
        {
            if (curr->key == key)
            {
                curr->next=curr->next->next;
                pthread_mutex_unlock(&L->lock);
                return 0; // success
            }
            curr = curr->next;
        }
        pthread_mutex_unlock(&L->lock);
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
    pthread_mutex_unlock(&L->lock);
    return -1; // failure
}

void *listTest(void *list)
{
    list_t *li;
    li = (list_t *)list;
    for (int i = 0; i < 100; i++)
    {
        List_Insert(li, i);
    }
}

void *listTest1(void *list)
{
    list_t *li;
    li = (list_t *)list;
    for (int i = 0; i < 200; i++)
    {
        List_Delete(li, i);
    }
}

int main(int argc, char const *argv[])
{
    list_t list;
    list_t *ptrList;
    ptrList = &list;

    List_Init(ptrList);
    // int x=List_Insert(ptrList,1);
    // x=List_Delete(ptrList,1);
    // x=List_Lookup(ptrList,1);
    // printf("%d",x);
    // printf("\n");

    pthread_t t1, t2;

    pthread_create(&t1, NULL, listTest, (void *)ptrList);
    pthread_create(&t2, NULL, listTest1, (void *)ptrList);

    //pthread_join(t1, NULL);
    //pthread_join(t2, NULL);


    for (int i = 0; i < 200; i++)
    {
        int x = List_Lookup(ptrList, i);
        printf("For Value ");
        printf("%d",i);
        printf(" the result of Lookup is: ");
        printf("%d", x);
        printf("\n");
    }
    return 0;
}