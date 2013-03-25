/* list.h */

#ifndef LIST_H
#define LIST_H

typedef struct node_t {
    void*          value;
    struct node_t*  next;
} node_t;

typedef struct {
    int             count;
    struct node_t*  head;
    struct node_t*  tail;
} list_t;

list_t*  ListCreate();
void  ListInsert(list_t*, void*);
void  ListFree(list_t*);
int   ListEmpty(list_t*);
void* ListRemoveRandom(list_t*);


#endif

