#ifndef PROJECT_INCLUDE_GROUP_OF_NETWORKS_H_
#define PROJECT_INCLUDE_GROUP_OF_NETWORKS_H_

#include "server.h"

#define RESIZE_RATIO 2
#define NO_GROUP_MSG "No group provided!\n"

typedef struct network {
    unsigned size;
    unsigned mem_size;
    uchar address[IP_SIZE];
    server_t *servers;
} network_t;

typedef struct group {
    unsigned size;
    network_t *nets;
} group_t;

int init_network(network_t *net);

int destroy_network(network_t *net);

int init_group(group_t *group);

int destroy_group(group_t *group);

int add_to_network(network_t *net, const server_t *server);

int add_to_group(group_t *group, const server_t *server);

int print_by_networks(const group_t *group);

#endif  // PROJECT_INCLUDE_GROUP_OF_NETWORKS_H_
