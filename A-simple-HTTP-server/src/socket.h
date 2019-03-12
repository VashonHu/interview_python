//
// Created by hu on 4/5/17.
//
#ifndef CONF_SOCKET_H
#define CONF_SOCKET_H

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
#include "dbg.h"

#define LISTENQ     1024
#define HOSTNAMELEN    30
#define PORTLEN        20

int open_listenfd(char* port);
int make_socket_non_blocking(int fd);
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);

#endif //CONF_SOCKET_H
