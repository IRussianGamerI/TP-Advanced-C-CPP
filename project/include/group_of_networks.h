#ifndef PROJECT_INCLUDE_GROUP_OF_NETWORKS_H_
#define PROJECT_INCLUDE_GROUP_OF_NETWORKS_H_

#include "server.h"

typedef struct Network {
    unsigned size;
    unsigned mem_size;
    uchar address[4];
    Server *servers;
} Network;

typedef struct Group {
    unsigned size;
    Network *nets;
} Group;

int init_network(Network *net);

int destroy_network(Network *net);

int init_group(Group *group);

int destroy_group(Group *group);

int add_to_network(Group *group, const Server *server);

int print_by_networks(const Group *group);

#endif  // PROJECT_INCLUDE_GROUP_OF_NETWORKS_H_
