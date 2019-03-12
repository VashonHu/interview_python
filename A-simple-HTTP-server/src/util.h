#ifndef UTIL_H
#define UTIL_H

// max number of listen queue

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <error.h>
#include <semaphore.h>
#include <semaphore.h>
#include <pthread.h>
#include "dbg.h"

#define BUFLEN      8192

#define DELIM       "="

#define NX_CONF_OK      0
#define NX_CONF_ERROR   100

#define MIN(a,b) ((a) < (b) ? (a) : (b))

struct nx_conf_s {
    void *root;
    char *port;
    int thread_num;
};

typedef struct nx_conf_s nx_conf_t;

char * ltrim(char *str);
u_char *ltrim_u(u_char *str);
int read_conf(char *filename, nx_conf_t *cf, char *buf, int len);
void Sem_init(sem_t *sem, int pshared, unsigned int value);
void P(sem_t *sem);
void V(sem_t *sem);
void Pthread_detach(pthread_t tid);


#endif
