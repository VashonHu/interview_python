#include <sys/time.h>
#include "timer.h"

static int timer_comp(void *ti, void *tj) {
    nx_timer_node *timeri = (nx_timer_node *)ti;
    nx_timer_node *timerj = (nx_timer_node *)tj;

    return (timeri->key < timerj->key)? 1: 0;
}

nx_pq_t nx_timer;
size_t nx_current_msec;

static void nx_time_update() {
    // there is only one thread calling nx_time_update, no need to lock?
    struct timeval tv;

    int rc = gettimeofday(&tv, NULL);
    check(rc == 0, "nx_time_update: gettimeofday error");

    nx_current_msec = (size_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
    debug("in nx_time_update, time = %zu", nx_current_msec);
}


int nx_timer_init() {
    int rc = nx_pq_init(&nx_timer, timer_comp, NX_PQ_DEFAULT_SIZE);
//    int rc = nx_timer_container_init(&nx_timer, timer_comp, NX_PQ_DEFAULT_SIZE);
    check(rc == NX_OK, "nx_pq_init error");
   
    nx_time_update();
    return NX_OK;
}

//return the rest of time of min time
int nx_find_timer() {
    nx_timer_node *timer_node;
    int time = NX_TIMER_INFINITE;
    int rc;

    while (!nx_pq_is_empty(&nx_timer)) {
        debug("nx_find_timer");
        nx_time_update();
//        timer_node = (nx_timer_node *)nx_timer_min(&nx_timer);
        timer_node = (nx_timer_node *)nx_pq_min(&nx_timer);
        check(timer_node != NULL, "nx_pq_min error");

        if (timer_node->deleted) {
            rc = nx_pq_delmin(&nx_timer); 
            check(rc == 0, "nx_pq_delmin");
            free(timer_node);
            continue;
        }
             
        time = (int) (timer_node->key - nx_current_msec);
        debug("in nx_find_timer, key = %zu, cur = %zu",
                timer_node->key,
                nx_current_msec);
        time = (time > 0? time: 0);
        break;
    }
    
    return time;
}

void nx_handle_expire_timers() {
    debug("in nx_handle_expire_timers");
    nx_timer_node *timer_node;
    int rc;

    while (!nx_pq_is_empty(&nx_timer)) {
        debug("nx_handle_expire_timers, size = %zu", nx_pq_size(&nx_timer));
        nx_time_update();
        timer_node = (nx_timer_node *)nx_pq_min(&nx_timer);
        check(timer_node != NULL, "nx_pq_min error");

        if (timer_node->deleted) {
            rc = nx_pq_delmin(&nx_timer); 
            check(rc == 0, "nx_handle_expire_timers: nx_pq_delmin error");
            free(timer_node);
            continue;
        }
        
        if (timer_node->key > nx_current_msec) {//no expire
            return;
        }

        if (timer_node->handler) {
            timer_node->handler((void*)timer_node->rq);
        }
        rc = nx_pq_delmin(&nx_timer); 
        check(rc == 0, "nx_handle_expire_timers: nx_pq_delmin error");
        free(timer_node);
    }
}

void nx_add_timer(nx_http_request_t *rq, size_t timeout, timer_handler_pt handler) {
    int rc;
    nx_timer_node *timer_node = (nx_timer_node *)malloc(sizeof(nx_timer_node));
    check(timer_node != NULL, "nx_add_timer: malloc error");

    nx_time_update();
    rq->timer = timer_node;
    timer_node->key = nx_current_msec + timeout;
    debug("in nx_add_timer, key = %zu", timer_node->key);
    timer_node->deleted = 0;
    timer_node->handler = handler;
    timer_node->rq = rq;

    rc = nx_pq_insert(&nx_timer, timer_node);
    check(rc == 0, "nx_add_timer: nx_pq_insert error");
}

void nx_del_timer(nx_http_request_t *rq) {
    debug("in nx_del_timer");
    nx_time_update();
    nx_timer_node *timer_node = rq->timer;
    check(timer_node != NULL, "nx_del_timer: rq->timer is NULL");

    timer_node->deleted = 1;
}
