//
// Created by hu on 4/6/17.
//
#include "threadpool.h"

void *worker(void *arg);

threadpool_t * threadpool_init(int n)
{
    threadpool_t *tp = (threadpool_t *)malloc(sizeof(threadpool_t));
    check_exit(tp != NULL, "out of space, initialize threadpool failed.");

    tp->tasks = (task_t **)malloc(sizeof(task_t *) * n);
    check_exit(tp->tasks != NULL, "out of space, initialize task array failed.");

    tp->n = n;
    tp->rear = tp->front = 0;
    Sem_init(&tp->mutex, 0, 1);
    Sem_init(&tp->slots, 0, n);
    Sem_init(&tp->items, 0, 0);

    pthread_t tmp;
    int rc;
    for(int i = 0; i < n; ++i) {
        rc = pthread_create(&tmp, NULL, worker, tp);
        check_exit(rc == 0, "create worker failed.");
    }

    return tp;
}


void threadpool_destroy(threadpool_t *tp)
{
    if(tp != NULL)
        free(tp->tasks);
}
void threadpool_add(threadpool_t *tp, func_t func, void *arg)
{
    task_t *task = (task_t *)malloc(sizeof(task_t));
    check(task != NULL, "out of space, threadpoll_add failed.");
    task->func = func;
    task->arg = arg;
    task->pthread_id = 0;

    P(&tp->slots);
    P(&tp->mutex);
    tp->tasks[(++tp->rear)%(tp->n)] = task;
    V(&tp->mutex);
    V(&tp->items);
}
task_t *threadpool_remove(threadpool_t *tp)
{
    task_t *task;
    P(&tp->items);
    P(&tp->mutex);
    task = tp->tasks[(++tp->front)%(tp->n)];
    V(&tp->mutex);
    V(&tp->slots);
    return task;
}

void *worker(void *threadpool)
{
    threadpool_t * tp = (threadpool_t *)threadpool;
    check_exit(tp != NULL, "threadpool is null !");

    pthread_t pthread_id = pthread_self();
    Pthread_detach(pthread_id);
    log_info("task %u start", (int)pthread_id);

    while(1){
        task_t * task = (task_t *)threadpool_remove(tp);
        check(task != NULL, "threadpool_remove failed");

        if (task->pthread_id == 0)
            task->pthread_id = pthread_id;

        task->func(task->arg);
    }
}