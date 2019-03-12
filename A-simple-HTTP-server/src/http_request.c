#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <math.h>
#include <time.h>
#include <unistd.h>
#include "http.h"
#include "http_request.h"
#include "error.h"

static int nx_http_process_ignore(nx_http_request_t *r, nx_http_out_t *out, char *data, int len);
static int nx_http_process_connection(nx_http_request_t *r, nx_http_out_t *out, char *data, int len);
static int nx_http_process_if_modified_since(nx_http_request_t *r, nx_http_out_t *out, char *data, int len);

nx_http_header_handle_t nx_http_headers_in[] = {//set the attribute
    {"Host", nx_http_process_ignore},
    {"Connection", nx_http_process_connection},
    {"If-Modified-Since", nx_http_process_if_modified_since},
    {"", nx_http_process_ignore}
};

int nx_init_request_t(nx_http_request_t *r, int fd, int epfd, nx_conf_t *cf)
{
    r->fd = fd;
    r->epfd = epfd;
    r->pos = r->last = 0;
    r->state = 0;//start state
    r->root = cf->root;
    INIT_LIST_HEAD(&(r->list));

    return NX_OK;
}

int nx_free_request_t(nx_http_request_t *r) {

    (void) r;
    return NX_OK;
}

int nx_init_out_t(nx_http_out_t *o, int fd) {
    o->fd = fd;
    o->keep_alive = 0;
    o->modified = 1;
    o->status = 0;

    return NX_OK;
}

int nx_free_out_t(nx_http_out_t *o) {
    // TODO
    (void) o;
    return NX_OK;
}

void nx_http_handle_header(nx_http_request_t *r, nx_http_out_t *o) {
    list_head *pos;
    nx_http_header_t *hd;
    nx_http_header_handle_t *header_in;
    int len;

    list_for_each(pos, &(r->list)) {//request's headers
        hd = list_entry(pos, nx_http_header_t, list);
        /* handle */

        for (header_in = nx_http_headers_in;//headers handle array
            strlen(header_in->name) > 0;
            header_in++) {
            if (strncmp(hd->key_start, header_in->name, hd->key_end - hd->key_start) == 0) {
            
                //debug("key = %.*s, value = %.*s", hd->key_end-hd->key_start, hd->key_start, hd->value_end-hd->value_start, hd->value_start);
                len = hd->value_end - hd->value_start;
                (*(header_in->handler))(r, o, hd->value_start, len);//call handle function
                break;
            }    
        }

        /* delete it from the original list */
        list_del(pos);
        free(hd);
    }
}

void * nx_http_close_conn(void *ptr) {
    // NOTICE: closing a file descriptor will cause it to be removed from all epoll sets automatically
    nx_http_request_t *r = (nx_http_request_t *)ptr;
    close(r->fd);
    free(r);

    return NULL;
}

static int nx_http_process_ignore(nx_http_request_t *r, nx_http_out_t *out, char *data, int len) {
    (void) r;
    (void) out;
    (void) data;
    (void) len;
    
    return NX_OK;
}

static int nx_http_process_connection(nx_http_request_t *r, nx_http_out_t *out, char *data, int len) {
    (void) r;
    if (strncasecmp("keep-alive", data, len) == 0) {
        out->keep_alive = 1;
    }

    return NX_OK;
}

static int nx_http_process_if_modified_since(nx_http_request_t *r, nx_http_out_t *out, char *data, int len) {
    (void) r;
    (void) len;
    (void)out;
    (void)data;
//
//    struct tm tm;
//    if (strptime(data, "%a, %d %b %Y %H:%M:%S GMT", &tm) == (char *)NULL) {
//        return NX_OK;
//    }
//    time_t client_time = mktime(&tm);
//
//    double time_diff = difftime(out->mtime, client_time);
//    if (fabs(time_diff) < 1e-6) {
//        // log_info("content not modified clienttime = %d, mtime = %d\n", client_time, out->mtime);
//        /* Not modified */
//        out->modified = 0;
//        out->status = NX_HTTP_NOT_MODIFIED;
//    }
//
    return NX_OK;
}

const char *get_shortmsg_from_status_code(int status_code) {
    if (status_code == NX_HTTP_OK) {
        return "OK";
    }

    if (status_code == NX_HTTP_NOT_MODIFIED) {
        return "Not Modified";
    }

    if (status_code == NX_HTTP_NOT_FOUND) {
        return "Not Found";
    }
    

    return "Unknown";
}
