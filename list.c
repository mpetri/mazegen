#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"

list_t*
ListCreate()
{
    list_t* plist = malloc(sizeof(list_t));

    if (plist==NULL) {
        perror("ListCreate malloc()");
        exit(1);
    }

    plist->count = 0;
    plist->head = NULL;

    return plist;
}

void
ListInsert(list_t* plist,void* v)
{
    node_t*  new, *current;

    assert(plist != NULL);

    if ((new = malloc(sizeof(node_t))) == NULL) {
        fprintf(stderr, "Memory allocation for list insert failed!\n");
        exit(1);
    }

    new->value = v;
    new->next = NULL;

    if (plist->count == 0) {
        plist->head = new;
    } else {
        current = plist->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new;
    }
    plist->count++;
}


void
ListFree(list_t* plist)
{
    node_t*         current, *next;
    assert(plist != NULL);
    /*
     * Need to systematically link through the free()ing the memory used
     * by nodes, but not before copying any required information out of
     * the node. eg. pointer to next node. so that we can continue to
     * free the list
     */
    current = plist->head;
    while (current != NULL) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void*
ListRemoveRandom(list_t* plist)
{
    node_t* cur,*prev;
    void* v;
    int i,rnd;

    assert(plist != NULL);
    rnd = rand() % plist->count;

    i = 0;
    cur = plist->head;
    prev = NULL;
    while (i<rnd) {
        i++;
        prev = cur;
        cur = cur->next;
    }

    if (cur) {
        if (prev) {
            prev->next = cur->next;
        } else {
            plist->head = cur->next;
        }
        v = cur->value;
        free(cur);
        plist->count--;
    }

    return v;
}

int
ListEmpty(list_t* plist)
{
    if (plist->count > 0) return 0;
    return 1;
}
