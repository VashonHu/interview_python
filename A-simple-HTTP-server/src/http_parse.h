#ifndef HTTP_PARSE_H
#define HTTP_PARSE_H

#define CR '\r'
#define LF '\n'
#define CRLFCRLF "\r\n\r\n"

int nx_http_parse_request_line(nx_http_request_t *r);
int nx_http_parse_request_body(nx_http_request_t *r);

#endif
