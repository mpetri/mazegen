#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "graph.h"
#include "list.h"

/* defined in mazegen.c */
extern void draw_maze(int,int);

graph_t*
graph_init()
{
    graph_t*        graph;
    if ((graph = malloc(sizeof(graph_t))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    graph->order = 0;
    graph->adjlists = NULL;

    return graph;
}

void
graph_create(graph_t* graph,int w,int h)
{
    int i,j,id;
    vertex_t* new,*cur,*tmp;

    graph->order = w*h;
    if ((graph->adjlists = malloc(sizeof(vertex_t*) * graph->order)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* create vertex for each x/y position in grid */
    id = 0;
    for (j=0; j<h; j++) {
        for (i=0; i<w; i++) {
            if ((new = malloc(sizeof(vertex_t))) == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            new->id = id;
            new->x = i;
            new->y = j;
            new->visited = 0;
            new->left = NULL;
            new->right = NULL;
            new->top = NULL;
            new->down = NULL;
            graph->adjlists[id] = new;
            id++;
        }
    }

    /* connect the adjacent vertices */
    for (i=0; i<graph->order; i++) {
        cur = graph->adjlists[i];
        if (i%w!= 0) {
            tmp = graph->adjlists[i-1];
            cur->left = tmp;
        }
        if (i%w!= w-1) {
            tmp = graph->adjlists[i+1];
            cur->right = tmp;
        }
        if (i>w) {
            tmp = graph->adjlists[i-w];
            cur->top = tmp;
        }
        if (i<graph->order-w) {
            tmp = graph->adjlists[i+w];
            cur->down = tmp;
        }
    }
}


void
graph_free(graph_t* graph)
{
    int             i;
    vertex_t*       tmp;

    assert(graph != NULL);
    assert(graph->adjlists != NULL);

    for (i = 0; i < graph->order; i++) {
        if (graph->adjlists[i]) {
            tmp = graph->adjlists[i];
            free(tmp);
        }
    }

    free(graph->adjlists);
    free(graph);
}

void
graph_addajacent(vertex_t* v,list_t* l)
{
    if (v->left && v->left->visited != 1) ListInsert(l,(void*)v->left);
    if (v->right && v->right->visited != 1) ListInsert(l,(void*)v->right);
    if (v->top && v->top->visited != 1) ListInsert(l,(void*)v->top);
    if (v->down && v->down->visited != 1) ListInsert(l,(void*)v->down);
}

void
graph_select(vertex_t* wall,vertex_t* opposite,list_t* wlist)
{
    draw_maze(wall->x,wall->y);
    draw_maze(opposite->x,opposite->y);
    wall->visited = 1;
    opposite->visited = 1;
    graph_addajacent(opposite,wlist);
}

void
graph_genmaze(graph_t* graph)
{
    int i;
    vertex_t* cur;
    list_t* wlist;

    wlist = ListCreate();

    /* pick random vertex as starting point*/
    i = rand() % graph->order;
    cur = graph->adjlists[i];
    draw_maze(cur->x,cur->y);
    /* mark as part of the maze */
    cur->visited = 1;

    /* add adjacent vertices to list of walls */
    graph_addajacent(cur,wlist);

    /* while candidate walls are available */
    while (!ListEmpty(wlist)) {
        /* choose random candidate */
        cur = (vertex_t*) ListRemoveRandom(wlist);

        /* check if the on opposite side is not part of the maze yet*/
        if (cur->left && cur->left->visited == 0 &&
            cur->right && cur->right->visited == 1) {
            graph_select(cur,cur->left,wlist);
        } else if (cur->right && cur->right->visited == 0 &&
                   cur->left && cur->left->visited == 1) {
            graph_select(cur,cur->right,wlist);
        } else if (cur->top && cur->top->visited == 0 &&
                   cur->down && cur->down->visited == 1) {
            graph_select(cur,cur->top,wlist);
        } else if (cur->down && cur->down->visited == 0 &&
                   cur->top && cur->top->visited == 1) {
            graph_select(cur,cur->down,wlist);
        }
    }

    ListFree(wlist);
}
