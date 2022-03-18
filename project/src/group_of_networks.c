#include "group_of_networks.h"

#include <malloc.h>
#include "server.h"

int init_network(network_t *net) {
    if (!net) {
        return NULLPTR_ERROR;
    }
    net->size = 0;
    net->mem_size = 1;
    net->servers = malloc(sizeof(server_t));
    if (!net->servers) {
        return MEMORY_ERROR;
    }
    return SUCCESS;
}

int destroy_network(network_t *net) {
    if (!net) {
        return NULLPTR_ERROR;
    }
    free(net->servers);
    return SUCCESS;
}

int init_group(group_t *group) {
    if (!group) {
        return NULLPTR_ERROR;
    }
    group->size = 0;
    group->nets = NULL;
    return SUCCESS;
}

int destroy_group(group_t *group) {
    if (!group) {
        return NULLPTR_ERROR;
    }
    for (unsigned i = 0; i < group->size; ++i) {
        free(group->nets[i].servers);
    }
    free(group->nets);
    return SUCCESS;
}

int add_to_network(network_t *net, const server_t *server) {
    if (!net || !server) {
        return NULLPTR_ERROR;
    }
    if (!net->servers) {
        return NULLPTR_ERROR;
    }
    if (net->size == net->mem_size) {
        server_t *ptr = realloc(net->servers, sizeof(server_t) * net->mem_size * RESIZE_RATIO);
        if (!ptr) {
            return MEMORY_ERROR;
        }
        net->servers = ptr;
        net->mem_size *= RESIZE_RATIO;
    }
    ++net->size;
    return init_server(&net->servers[net->size - 1], server->dns, server->ip,
                       server->netmask, server->cpus, server->cores);
}

int add_to_group(group_t *group, const server_t *server) {
    if (!group || !server) {
        return NULLPTR_ERROR;
    }
    uchar *net_address = malloc(IP_SIZE * sizeof(uchar));
    if (!net_address) {
        return MEMORY_ERROR;
    }
    get_network_address(server, net_address);
    for (unsigned i = 0; i < group->size; ++i) {
        if (compare_ip(group->nets[i].address, net_address) == 1) {
            if (add_to_network(&group->nets[i], server) == SUCCESS) {
                free(net_address);
                return SUCCESS;
            }
        }
    }
    free(net_address);
    network_t *ptr = realloc(group->nets, (group->size + 1) * sizeof(network_t));
    if (!ptr) {
        return MEMORY_ERROR;
    }
    group->nets = ptr;
    if (init_network(&group->nets[group->size])) {
        return MEMORY_ERROR;
    }
    ++group->size;
    get_network_address(server, group->nets[group->size - 1].address);
    return add_to_network(&group->nets[group->size - 1], server);
}

int print_by_networks(const group_t *group) {
    if (!group) {
        printf(NO_GROUP_MSG);
        return NULLPTR_ERROR;
    }
    if (!group->nets) {
        return NULLPTR_ERROR;
    }
    for (unsigned i = 0; i < group->size; ++i) {
        if (!group->nets[i].servers) {
            return NULLPTR_ERROR;
        }
        printf("\n=========================================\nNETWORK: %hhu.%hhu.%hhu.%hhu\n",
               group->nets[i].address[0],
               group->nets[i].address[1],
               group->nets[i].address[RESIZE_RATIO],
               group->nets[i].address[3]);
        for (unsigned j = 0; j < group->nets[i].size; ++j) {
            print_server(&group->nets[i].servers[j]);
        }
    }
    return SUCCESS;
}
