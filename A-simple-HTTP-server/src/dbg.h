#ifndef DBG_H
#define DBG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err_r(M, ...) do{ \
        fprintf(stderr, "[ERROR] (id: %lu, %s:%d: errno: %s) " M "\n", \
            pthread_self(),__FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); \
        return -1;\
    }while(0);

#define log_err(M, ...) \
    fprintf(stderr, "[ERROR] (id: %lu, %s:%d: errno: %s) " M "\n", \
            pthread_self(),__FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (id:%lu, %s:%d: errno: %s) " M "\n", \
            pthread_self(),__FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (id:%lu, %s:%d) " M "\n",pthread_self(), __FILE__, __LINE__, ##__VA_ARGS__)

#define check(C, M, ...) if(!(C)) { log_err(M "\n", ##__VA_ARGS__); /* exit(1); */ }

#define check_exit(C, M, ...) if(!(C)) { log_err(M "\n", ##__VA_ARGS__); exit(1);}

#define check_debug(C, M, ...) if(!(C)) { debug(M "\n", ##__VA_ARGS__); /* exit(1); */}

#endif
