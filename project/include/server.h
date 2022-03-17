#ifndef PROJECT_INCLUDE_SERVER_H_
#define PROJECT_INCLUDE_SERVER_H_

#include <stdio.h>

#define BUF_SIZE 16

enum ERROR_CODES {
    SUCCESS = 0,
    INPUT_ERROR,
    INCORRECT_INPUT,
    NULLPTR_ERROR,
    MEMORY_ERROR,
    INCORRECT_NUMBER
};

typedef unsigned char uchar;

typedef struct Server {
    uchar dns[4];
    uchar ip[4];
    uchar netmask[4];
    int cpus;
    int cores;
} Server;

int init_server(Server *server, const uchar dns[], const uchar ip[], const uchar netmask[], int cpus, int cores);

int extract_ip(const char *from, uchar *to);

int read_ip(FILE *in, uchar *ip, const char *msg);

int str_count(const char *str, char sym);

int get_network_address(const Server *server, uchar *result);

int compare_ip(const uchar *ip1, const uchar *ip2);

int print_server(const Server *server);

#endif  // PROJECT_INCLUDE_SERVER_H_
