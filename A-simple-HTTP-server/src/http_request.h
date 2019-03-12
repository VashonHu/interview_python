#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <time.h>
#include <stdbool.h>
#include "http.h"
#include "socket.h"
#define NX_AGAIN    EAGAIN

#define NX_HTTP_PARSE_INVALID_METHOD        10
#define NX_HTTP_PARSE_INVALID_REQUEST       11
#define NX_HTTP_PARSE_INVALID_HEADER        12

#define NX_HTTP_UNKNOWN                     0x0001
#define NX_HTTP_GET                         0x0002
#define NX_HTTP_HEAD                        0x0004
#define NX_HTTP_POST                        0x0008

#define NX_HTTP_OK                          200

#define NX_HTTP_NOT_MODIFIED                304

#define NX_HTTP_NOT_FOUND                   404

#define MAX_BUF 8124

typedef struct nx_http_request_s {
    void *root;
    int fd;
    int epfd;
    char hostname[HOSTNAMELEN];
    char port[PORTLEN];
    char buf[MAX_BUF];  /* ring buffer */
    char out[MAX_BUF];
    size_t pos, last;
    int state;

    void *request_start;
    void *method_end;   /* not include method_end*/
    int method;
    void *uri_start;
    void *uri_end;      /* not include uri_end*/ 
    void *path_start;
    void *path_end;
    void *query_start;
    void *query_end;
    int http_major;
    int http_minor;
    void *request_end;

    struct list_head list;  /* store http header */
    void *cur_header_key_start;
    void *cur_header_key_end;
    void *cur_header_value_start;
    void *cur_header_value_end;

    void *timer;
} nx_http_request_t;

typedef struct {
    int fd;
    int keep_alive;
    time_t mtime;       /* the modified time of the file*/
    int modified;       /* compare If-modified-since field with mtime to decide whether the file is modified since last time*/

    int status;
} nx_http_out_t;//

typedef struct nx_http_header_s {
    void *key_start, *key_end;          /* not include end */
    void *value_start, *value_end;
    list_head list;
} nx_http_header_t;

typedef int (*nx_http_header_handler_pt)(nx_http_request_t *r, nx_http_out_t *o, char *data, int len);

typedef struct {
    char *name;
    nx_http_header_handler_pt handler;
} nx_http_header_handle_t;

void nx_http_handle_header(nx_http_request_t *r, nx_http_out_t *o);
void * nx_http_close_conn(void *ptr);

int nx_init_request_t(nx_http_request_t *r, int fd, int epfd, nx_conf_t *cf);
int nx_free_request_t(nx_http_request_t *r);

int nx_init_out_t(nx_http_out_t *o, int fd);
int nx_free_out_t(nx_http_out_t *o);

const char *get_shortmsg_from_status_code(int status_code);

extern nx_http_header_handle_t     nx_http_headers_in[];

#endif

