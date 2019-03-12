#ifndef NX_TIMER_H
#define NX_TIMER_H

#include "priority_queue.h"
#include "http_request.h"

#define NX_TIMER_INFINITE -1
#define TIMEOUT_DEFAULT 500     /* ms */
#define REPEAT_READ 50
#define REPEAT_WRITE 50

typedef void * (*timer_handler_pt)(void *rq);

typedef struct nx_timer_node_s{
    size_t key;
    int deleted;    /* if remote client close the socket first, set deleted to 1 */
    timer_handler_pt handler;
    nx_http_request_t *rq;
} nx_timer_node;

int nx_timer_init();
int nx_find_timer();
void nx_handle_expire_timers();

extern nx_pq_t nx_timer;
//#define nx_timer_container_init nx_pq_init;
//#define nx_timer_is_empty nx_pq_is_empty ;
//#define nx_timer_size nx_pq_size ;
//#define nx_timer_min nx_pq_min;
//#define nx_timer_delmin nx_pq_delmin ;
//#define nx_timer_insert nx_pq_insert ;
extern size_t nx_current_msec;

void nx_add_timer(nx_http_request_t *rq, size_t timeout, timer_handler_pt handler);
void nx_del_timer(nx_http_request_t *rq);

#endif
