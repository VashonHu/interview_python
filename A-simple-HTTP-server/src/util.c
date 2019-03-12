
#include "util.h"

char * ltrim(char *str)
{
    if(str == NULL || *str == '\n' || *str == '\0')
        return str;

    while(*str == ' ')
        str++;

    return str;
}

u_char *ltrim_u(u_char *str)
{
    if(str == NULL || *str == '\n' || *str == '\0')
        return str;

    while(*str == ' ')
        str++;

    return str;
}


/*
* Read configuration file
*/
int read_conf(char *filename, nx_conf_t *cf, char *buf, int len) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        log_err("cannot open config file: %s", filename);
        return NX_CONF_ERROR;
    }

    int pos = 0;
    char *delim_pos;
    size_t line_len;
    char *cur_pos = buf+pos;

    while (fgets(cur_pos, len-pos, fp)) {//一直读，直到遇到'\n'
        delim_pos = strstr(cur_pos, DELIM);
        line_len = strlen(cur_pos);
        
//        /*
//        log_info("read one line from conf: %s, len = %d", cur_pos, line_len);
//        */
        if (!delim_pos)
            return NX_CONF_ERROR;
        
        if (cur_pos[strlen(cur_pos) - 1] == '\n') {
            cur_pos[strlen(cur_pos) - 1] = '\0';//以继续读
        }

        cur_pos = ltrim(cur_pos);//delete left ' '

        if (strncmp("root", cur_pos, 4) == 0) {
            cf->root = delim_pos + 1;
        }

        if (strncmp("port", cur_pos, 4) == 0) {
            cf->port = delim_pos + 1;
        }

        if (strncmp("threadnum", cur_pos, 9) == 0) {
            cf->thread_num = atoi(delim_pos + 1);
        }

        cur_pos += line_len;
    }

    fclose(fp);
//    fclose(fp);
    return NX_CONF_OK;
}


void Sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (sem_init(sem, pshared, value) < 0)
        log_err("Sem_init error");
}

void P(sem_t *sem)
{
    if (sem_wait(sem) < 0)
        log_err("P error");
}

void V(sem_t *sem)
{
    if (sem_post(sem) < 0)
        log_err("V error");
}

void Pthread_detach(pthread_t tid) {
    int rc;

    rc = pthread_detach(tid);
    check(rc == 0, "Pthread_detach error");
}

//
//int main()
//{
//    char conf_buf[BUFLEN];
//    nx_conf_t cf;
//
//    read_conf("../nx.conf", &cf, conf_buf, BUFLEN);
//}