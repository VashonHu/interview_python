#include "priority_queue.h"

int nx_pq_init(nx_pq_t *nx_pq, nx_pq_comparator_pt comp, size_t size) {
    nx_pq->pq = (void **)malloc(sizeof(void *) * (size+1));
    if (!nx_pq->pq)
        log_err_r("nx_pq_init: malloc failed");
    
    nx_pq->nalloc = 0;
    nx_pq->size = size + 1;
    nx_pq->comp = comp;
    
    return NX_OK;
}

int nx_pq_is_empty(nx_pq_t *nx_pq) {
    return (nx_pq->nalloc == 0)? 1: 0;
}

size_t nx_pq_size(nx_pq_t *nx_pq) {
    return nx_pq->nalloc;
}

void *nx_pq_min(nx_pq_t *nx_pq) {
    if (nx_pq_is_empty(nx_pq)) {
        return NULL;
    }

    return nx_pq->pq[1];
}

static int resize(nx_pq_t *nx_pq, size_t new_size) {
    if (new_size <= nx_pq->nalloc) {
        log_err("resize: new_size to small");
        return -1;
    }

    void **new_ptr = (void **)malloc(sizeof(void *) * new_size);
    if (!new_ptr) {
        log_err("resize: malloc failed");
        return -1;
    }

    memcpy(new_ptr, nx_pq->pq, sizeof(void *) * (nx_pq->nalloc + 1));
    free(nx_pq->pq);
    nx_pq->pq = new_ptr;
    nx_pq->size = new_size;
    return NX_OK;
}

static void exch(nx_pq_t *nx_pq, size_t i, size_t j) {
    void *tmp = nx_pq->pq[i];
    nx_pq->pq[i] = nx_pq->pq[j];
    nx_pq->pq[j] = tmp;
}

static void swim(nx_pq_t *nx_pq, size_t k) {
    while (k > 1 && nx_pq->comp(nx_pq->pq[k], nx_pq->pq[k/2])) {
        exch(nx_pq, k, k/2);
        k /= 2;
    }
}

static size_t sink(nx_pq_t *nx_pq, size_t k) {
    size_t j;
    size_t nalloc = nx_pq->nalloc;

    while (2*k <= nalloc) {
        j = 2*k;
        if (j < nalloc && nx_pq->comp(nx_pq->pq[j+1], nx_pq->pq[j])) j++;
        if (!nx_pq->comp(nx_pq->pq[j], nx_pq->pq[k])) break;
        exch(nx_pq, j, k);
        k = j;
    }
    
    return k;
}

int nx_pq_delmin(nx_pq_t *nx_pq) {
    if (nx_pq_is_empty(nx_pq)) {
        return NX_OK;
    }

    exch(nx_pq, 1, nx_pq->nalloc);
    nx_pq->nalloc--;
    sink(nx_pq, 1);
    if (nx_pq->nalloc > 0 && nx_pq->nalloc <= (nx_pq->size - 1)/4) {
        if (resize(nx_pq, nx_pq->size / 2) < 0) {
            return -1;
        }
    }

    return NX_OK;
}

int nx_pq_insert(nx_pq_t *nx_pq, void *item) {
    if (nx_pq->nalloc + 1 == nx_pq->size) {
        if (resize(nx_pq, nx_pq->size * 2) < 0) {
            return -1;
        }
    }

    nx_pq->pq[++nx_pq->nalloc] = item;
    swim(nx_pq, nx_pq->nalloc);

    return NX_OK;
}

int nx_pq_sink(nx_pq_t *nx_pq, size_t i) {
    return sink(nx_pq, i);
}
