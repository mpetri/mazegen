/* graph.h */

#ifndef GRAPH_H

#include "list.h"

typedef struct vertex_t {
    int             id;
    int             x;
    int             y;
    int             visited;
    struct vertex_t* left;
    struct vertex_t* right;
    struct vertex_t* top;
    struct vertex_t* down;
} vertex_t;

typedef struct {
    int             order;
    vertex_t**      adjlists;
} graph_t;

void            graph_create();
graph_t*        graph_init();
void            graph_free();
void            graph_genmaze(graph_t* graph);
void            graph_select(vertex_t* wall,vertex_t* opposite,list_t* wlist);
void            graph_addajacent(vertex_t* v,list_t* l);
vertex_t*       graph_copy(vertex_t* v);

#endif
