#include "server.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int init_server(Server *server, const uchar dns[], const uchar ip[], const uchar netmask[], int cpus, int cores) {
    if (!server || !dns || !ip || !netmask) {
        return NULLPTR_ERROR;
    }
    memcpy(server->dns, dns, 4);
    memcpy(server->ip, ip, 4);
    memcpy(server->netmask, netmask, 4);
    server->cpus = cpus;
    server->cores = cores;
    return SUCCESS;
}

Server *create_server_array(unsigned size) {
    if (!size) {
        printf("Array could not be created\n");
        return NULL;
    }
    Server *array = malloc(size * sizeof(Server));
    if (!array) {
        printf("Unable to allocate memory\n");
        return NULL;
    }
    return array;
}

void free_server_array(Server *array) {
    free(array);
}

int read_ip(uchar *ip, char *msg) {
    char buf[BUF_SIZE];
    printf("Enter %s: ", msg);
    if (!scanf("%16s", buf)) {
        printf("Cannot read dns-name!\n");
        return INPUT_ERROR;
    }
    return extract_ip(buf, ip);
}

int str_count(char *str, char sym) {
    int cnt = 0;
    for (unsigned i = 0; i < strnlen(str, BUF_SIZE); ++i) {
        if (str[i] == sym) {
            ++cnt;
        }
    }
    return cnt;
}

int extract_ip(char *from, uchar *to) {
    if (str_count(from, '.') != 3) {
        printf("Entered IPv4 address is incorrect!\n");
        return INCORRECT_INPUT;
    }
    char *cur = strtok_r(from, ".", &from);
    if (strnlen(cur, 4) > 3) {
        printf("Entered IPv4 address is incorrect!\n");
        return INCORRECT_INPUT;
    }
    int byte;
    byte = strtoul(cur, NULL, 10);
    if (byte > 255) {
        printf("Entered IPv4 address is incorrect!\n");
        return INCORRECT_INPUT;
    }
    to[0] = byte;
    for (int i = 1; i < 4; ++i) {
        cur = strtok_r(NULL, ".", &from);
        if (strnlen(cur, 4) > 3) {
            printf("Entered IPv4 address is incorrect!\n");
            return INCORRECT_INPUT;
        }
        byte = strtoul(cur, NULL, 10);
        if (byte > 255) {
            printf("Entered IPv4 address is incorrect!\n");
            return INCORRECT_INPUT;
        }
        to[i] = byte;
    }
    return SUCCESS;
}

int get_network_address(const Server *server, uchar *result) {
    if (!result || !server) {
        return NULLPTR_ERROR;
    }
    for (int i = 0; i < 4; ++i) {
        result[i] = server->ip[i] & server->netmask[i];
    }
    return SUCCESS;
}

int compare_ip(const uchar *ip1, const uchar *ip2) {
    if (!ip1 || !ip2) {
        return NULLPTR_ERROR;
    }
    return (ip1[0] == ip2[0] &&
            ip1[1] == ip2[1] &&
            ip1[2] == ip2[2] &&
            ip1[3] == ip2[3]);
}

int print_server(const Server *server) {
    if (!server) {
        return NULLPTR_ERROR;
    }
    printf("--------------------------------------\n");
    printf("DNS: %hhu.%hhu.%hhu.%hhu\n", server->dns[0], server->dns[1], server->dns[2], server->dns[3]);
    printf("IP: %hhu.%hhu.%hhu.%hhu\n", server->ip[0], server->ip[1], server->ip[2], server->ip[3]);
    printf("Netmask: %hhu.%hhu.%hhu.%hhu\n",
           server->netmask[0], server->netmask[1], server->netmask[2], server->netmask[3]);
    printf("CPUs: %d\n", server->cpus);
    printf("Cores: %d\n", server->cores);
    return SUCCESS;
}


