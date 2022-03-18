#include "server.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int init_server(server_t *server, const uchar dns[], const uchar ip[], const uchar netmask[], int cpus, int cores) {
    if (!server || !dns || !ip || !netmask) {
        return NULLPTR_ERROR;
    }
    if (cpus < 0 || cores < 0) {
        return INCORRECT_NUMBER;
    }
    memcpy(server->dns, dns, IP_SIZE);
    memcpy(server->ip, ip, IP_SIZE);
    memcpy(server->netmask, netmask, IP_SIZE);
    server->cpus = cpus;
    server->cores = cores;
    return SUCCESS;
}

int read_ip(FILE *in, uchar *ip, const char *msg) {
    if (!in || !ip || !msg) {
        return NULLPTR_ERROR;
    }
    char buf[BUF_SIZE] = {0};
    printf("Enter %s: ", msg);
    if (fscanf(in, "%15s", buf) != 1) {
        printf("Cannot read dns-name!\n");
        return INPUT_ERROR;
    }
    return extract_ip(buf, ip);
}

int str_count(const char *str, char sym) {
    if (str == NULL || sym == '\0') {
        return 0;
    }
    int cnt = 0;
    for (unsigned i = 0; i < strnlen(str, BUF_SIZE); ++i) {
        if (str[i] == sym) {
            ++cnt;
        }
    }
    return cnt;
}

int extract_ip(const char *from, uchar *to) {
    if (!from || !to) {
        return NULLPTR_ERROR;
    }
    char *from_cpy = calloc(16, sizeof(char));
    char *origin = from_cpy;
    if (!from_cpy) {
        free(origin);
        return MEMORY_ERROR;
    }
    strncpy(from_cpy, from, 15);
    if (str_count(from_cpy, '.') != 3) {
        printf(IP_INCORRECT_MSG);
        free(origin);
        return INCORRECT_INPUT;
    }
    char *cur = strtok_r(from_cpy, ".", &from_cpy);
    if (!cur) {
        printf(IP_INCORRECT_MSG);
        free(origin);
        return INCORRECT_INPUT;
    }
    if (strnlen(cur, IP_SIZE) > 3) {
        printf(IP_INCORRECT_MSG);
        free(origin);
        return INCORRECT_INPUT;
    }
    unsigned byte;
    byte = strtoul(cur, NULL, 10);
    if (byte > 255) {
        printf(IP_INCORRECT_MSG);
        free(origin);
        return INCORRECT_INPUT;
    }
    to[0] = byte;
    for (int i = 1; i < IP_SIZE; ++i) {
        cur = strtok_r(from_cpy, ".", &from_cpy);
        if (strnlen(cur, IP_SIZE) > 3) {
            printf(IP_INCORRECT_MSG);
            free(origin);
            return INCORRECT_INPUT;
        }
        byte = strtoul(cur, NULL, 10);
        if (byte > 255) {
            printf(IP_INCORRECT_MSG);
            free(origin);
            return INCORRECT_INPUT;
        }
        to[i] = byte;
    }
    free(origin);
    return SUCCESS;
}

int get_network_address(const server_t *server, uchar *result) {
    if (!result || !server) {
        return NULLPTR_ERROR;
    }
    for (int i = 0; i < IP_SIZE; ++i) {
        result[i] = server->ip[i] & server->netmask[i];
    }
    return SUCCESS;
}

int compare_ip(const uchar *ip1, const uchar *ip2) {
    if (!ip1 || !ip2) {
        return 0;
    }
    return (ip1[0] == ip2[0] &&
            ip1[1] == ip2[1] &&
            ip1[2] == ip2[2] &&
            ip1[3] == ip2[3]);
}

int read_server(FILE *in, server_t *server) {
    if (read_ip(in, server->dns, "DNS") != SUCCESS) {
        return INPUT_ERROR;
    }

    if (read_ip(in, server->ip, "IP") != SUCCESS) {
        return INPUT_ERROR;
    }

    if (read_ip(in, server->netmask, "Netmask") != SUCCESS) {
        return INPUT_ERROR;
    }

    fprintf(stdout, "Enter the amount of CPUs: ");
    if (fscanf(in, "%d", &server->cpus) != 1) {
        return INPUT_ERROR;
    }

    fprintf(stdout, "Enter the amount of cores: ");
    if (fscanf(in, "%d", &server->cores) != 1) {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

int print_server(const server_t *server) {
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
