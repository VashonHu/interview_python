//#include <stdint.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include "util.h"
#include "timer.h"
//#include "http.h"
#include "epoll.h"
#include "threadpool.h"


#define CONF "nx.conf"
#define PROGRAM_VERSION "0.1"

extern struct epoll_event *events;

static const struct option long_options[]=
{
    {"help",no_argument,NULL,'?'},
    {"version",no_argument,NULL,'V'},
    {"conf",required_argument,NULL,'c'},
    {NULL,0,NULL,0}
};

static void usage() {
   fprintf(stderr,
	"nx [option]... \n"
	"  -c|--conf <config file>  Specify config file. Default ./nx.conf.\n"
	"  -?|-h|--help             This information.\n"
	"  -V|--version             Display program version.\n"
	);
}

int main(int argc, char* argv[]) {
    int rc;
    int opt = 0;
    int options_index = 0;
    char *conf_file = CONF;

    if (argc == 1) {
        usage();
        return 0;
    }

    while ((opt=getopt_long(argc, argv,"Vc:?h",long_options,&options_index)) != EOF) {
        switch (opt) {
            case  0 : break;
            case 'c':
                conf_file = optarg;
                break;
            case 'V':
                printf(PROGRAM_VERSION"\n");
                return 0;
            case ':':
            case 'h':
            case '?':
                usage();
                return 0;
        }
    }

//    debug("conffile = %s", conf_file);

    if (optind < argc) {
        log_err("non-option ARGV-elements: ");
        while (optind < argc)
            log_err("%s ", argv[optind++]);
        return 0;
    }

    /*
    * read confile file
    */
    char conf_buf[BUFLEN];
    nx_conf_t cf;
    rc = read_conf(conf_file, &cf, conf_buf, BUFLEN);
    check(rc == NX_CONF_OK, "read conf err");

    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, NULL))
        log_err_r("install signal handler for SIGPIPE failed");


    // initialize listening socket
    int listenfd;
    listenfd = open_listenfd(cf.port);
    rc = make_socket_non_blocking(listenfd);
    check_exit(rc == 0, "make_socket_non_blocking");

    /*
    * create epoll and add listenfd to ep
    */
    int epfd = nx_epoll_create(0);
    struct epoll_event event;
    
    nx_http_request_t *main_request = (nx_http_request_t *)malloc(sizeof(nx_http_request_t));
    nx_init_request_t(main_request, listenfd, epfd, &cf);

    event.data.ptr = (void *)main_request;
    event.events = EPOLLIN | EPOLLET;
    nx_epoll_add(epfd, listenfd, &event);

    // create thread pool
//    /*
//    threadpool_t *tp = threadpool_init(cf.thread_num);
//    check(tp != NULL, "threadpool_init error");
//    */

    //initialize timer
    nx_timer_init();

    log_info("nx started.");
    int n;
    int i, fd;
    int time;

    /* epoll_wait loop */
    while (1) {
        time = nx_find_timer();
        debug("wait time = %d", time);
        n = nx_epoll_wait(epfd, events, MAXEVENTS, time);

        struct sockaddr_storage clientaddr;
        socklen_t clientlen;
//        memset(&clientaddr, 0, sizeof(struct sockaddr_in));

        for (i = 0; i < n; i++) {
            nx_http_request_t *r = (nx_http_request_t *)events[i].data.ptr;
            fd = r->fd;

            if (listenfd == fd) {

                int infd;
                while(1) {
                    clientlen = sizeof(struct sockaddr_storage);
                    infd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
                    if (infd < 0) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            /* we have processed all incoming connections */
                            break;
                        } else {
                            log_err("accept");
                            break;
                        }
                    }

                    rc = make_socket_non_blocking(infd);
                    check(rc == 0, "make_socket_non_blocking");

                    /*log_info("new connection from %s:%d, fd:%d, %u", \
                      inet_ntop(AF_INET, (struct sockaddr *)&clientaddr, addr_p, sizeof(addr_p)), \
                      ntohl(clientaddr.sin_port), infd, clientaddr.sin_addr.s_addr);
                    */

                    nx_http_request_t *request = (nx_http_request_t *)malloc(sizeof(nx_http_request_t));
                    if (request == NULL) {
                        log_err("malloc(sizeof(nx_http_request_t))");
                        break;
                    }

                    nx_init_request_t(request, infd, epfd, &cf);
                    event.data.ptr = (void *)request;
                    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                    Getnameinfo((struct sockaddr *)&clientaddr, clientlen,request->hostname, HOSTNAMELEN, request->port, PORTLEN, 0);

                    log_info("new connection fron %s:%s:", request->hostname, request->port);

                    nx_epoll_add(epfd, infd, &event);
                    nx_add_timer(request, TIMEOUT_DEFAULT, nx_http_close_conn);
                }   // end of while of accept

            } else {
                if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN))) {
                    log_err("epoll error fd: %d", r->fd);
                    close(fd);
                    continue;
                }

                log_info("new data from %s:%s", r->hostname, r->port);
//                threadpool_add(tp, read_request, events[i].data.ptr);

                read_request(events[i].data.ptr);
            }
        }//end of for epoll_wait events

        nx_handle_expire_timers();//after handled new and old connection,handle expire timers
    }

//    threadpool_destroy(tp);

    return 0;
}
