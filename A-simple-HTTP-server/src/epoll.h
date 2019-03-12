#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#define MAXEVENTS 1024

int nx_epoll_create(int flags);
void nx_epoll_add(int epfd, int fs, struct epoll_event *event);
void nx_epoll_mod(int epfd, int fs, struct epoll_event *event);
void nx_epoll_del(int epfd, int fs, struct epoll_event *event);
int nx_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#endif
