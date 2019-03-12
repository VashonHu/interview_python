//
// Created by hu on 4/6/17.
//

#ifndef CONF_THREADPOOL_H
#define CONF_THREADPOOL_H

#include "http_request.h"
#include "util.h"

typedef void *(* func_t)(void *arg);

typedef struct
{
    func_t func;
    void *arg;
    pthread_t pthread_id;
}task_t;

typedef struct
{
    task_t **tasks;
    int n;//the sum of slots
    int front;// first item
    int rear;// last item
    sem_t mutex;
    sem_t slots;//counts available slots
    sem_t items;//counts available items
}threadpool_t;

threadpool_t * threadpool_init(int n);
void threadpool_destroy(threadpool_t *tp);
void threadpool_add(threadpool_t *tp, func_t func, void *arg);

#endif //CONF_THREADPOOL_H
