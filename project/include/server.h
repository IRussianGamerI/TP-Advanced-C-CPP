#ifndef PROJECT_INCLUDE_SERVER_H_
#define PROJECT_INCLUDE_SERVER_H_

#include <stdio.h>

#define BUF_SIZE 16
#define IP_SIZE 4
#define IP_INCORRECT_MSG "Entered IPv4 address is incorrect!\n"

enum ERROR_CODES {
    SUCCESS = 0,
    INPUT_ERROR,
    INCORRECT_INPUT,
    NULLPTR_ERROR,
    MEMORY_ERROR,
    INCORRECT_NUMBER
};

typedef unsigned char uchar;

typedef struct server {
    uchar dns[IP_SIZE];
    uchar ip[IP_SIZE];
    uchar netmask[IP_SIZE];
    int cpus;
    int cores;
} server_t;

int init_server(server_t *server, const uchar dns[], const uchar ip[], const uchar netmask[], int cpus, int cores);

int extract_ip(const char *from, uchar *to);

int read_ip(FILE *in, uchar *ip, const char *msg);

int str_count(const char *str, char sym);

int get_network_address(const server_t *server, uchar *result);

int compare_ip(const uchar *ip1, const uchar *ip2);

int read_server(FILE *in, server_t *server);

int print_server(const server_t *server);

#endif  // PROJECT_INCLUDE_SERVER_H_
