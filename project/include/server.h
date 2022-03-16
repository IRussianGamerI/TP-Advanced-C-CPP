#ifndef PROJECT_INCLUDE_SERVER_H_
#define PROJECT_INCLUDE_SERVER_H_

#define BUF_SIZE 16

enum ERROR_CODES {
    SUCCESS = 0,
    INPUT_ERROR,
    INCORRECT_INPUT,
    NULLPTR_ERROR,
    MEMORY_ERROR
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

Server *create_server_array(unsigned size);

void free_server_array(Server *array);

int extract_ip(char *from, uchar *to);

int read_ip(uchar *ip, char *msg);

int str_count(char *str, char sym);

int get_network_address(const Server *server, uchar *result);

int compare_ip(const uchar *ip1, const uchar *ip2);

int print_server(const Server *server);

#endif  // PROJECT_INCLUDE_SERVER_H_
