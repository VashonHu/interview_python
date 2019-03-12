#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "dbg.h"
#include "error.h"

#define NX_PQ_DEFAULT_SIZE 10

typedef int (*nx_pq_comparator_pt)(void *pi, void *pj);

typedef struct {
    void **pq;//the array
    size_t nalloc;//the size of used
    size_t size;// the size of array 
    nx_pq_comparator_pt comp;
} nx_pq_t;

int nx_pq_init(nx_pq_t *nx_pq, nx_pq_comparator_pt comp, size_t size);
int nx_pq_is_empty(nx_pq_t *nx_pq);
size_t nx_pq_size(nx_pq_t *nx_pq);
void *nx_pq_min(nx_pq_t *nx_pq);
int nx_pq_delmin(nx_pq_t *nx_pq);
int nx_pq_insert(nx_pq_t *nx_pq, void *item);

int nx_pq_sink(nx_pq_t *nx_pq, size_t i);
#endif 
