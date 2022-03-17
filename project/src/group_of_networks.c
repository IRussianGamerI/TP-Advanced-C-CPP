#include "group_of_networks.h"
#include <malloc.h>
#include "server.h"

int init_network(Network *net) {
    if (!net) {
        return NULLPTR_ERROR;
    }
    net->size = 0;
    net->mem_size = 0;
    net->servers = malloc(sizeof(Server));
    if (!net->servers){
        return MEMORY_ERROR;
    }
    return SUCCESS;
}

int destroy_network(Network *net) {
    if (!net) {
        return NULLPTR_ERROR;
    }
    free(net->servers);
    return SUCCESS;
}

int init_group(Group *group) {
    if (!group) {
        return NULLPTR_ERROR;
    }
    group->size = 0;
    group->nets = NULL;
    return SUCCESS;
}

int destroy_group(Group *group) {
    if (!group) {
        return NULLPTR_ERROR;
    }
    for (unsigned i = 0; i < group->size; ++i) {
        free(group->nets[i].servers);
    }
    free(group->nets);
    return SUCCESS;
}

int add_to_network(Group *group, const Server *server) {
    if (!group || !server) {
        return NULLPTR_ERROR;
    }
    uchar *net_address = malloc(4 * sizeof(uchar));
    if (!net_address) {
        return MEMORY_ERROR;
    }
    get_network_address(server, net_address);
    for (unsigned i = 0; i < group->size; ++i) {
        if (compare_ip(group->nets[i].address, net_address) == 1) {
            if (group->nets[i].size == group->nets[i].mem_size) {
                Server *ptr = realloc(group->nets[i].servers, sizeof(Server) * group->nets[i].mem_size * 2);
                if (!ptr) {
                    free(net_address);
                    return MEMORY_ERROR;
                }
                group->nets[i].servers = ptr;
                group->nets[i].mem_size *= 2;
            }
            ++group->nets[i].size;
            free(net_address);
            return init_server(&group->nets[i].servers[group->nets[i].size - 1], server->dns, server->ip,
                               server->netmask, server->cpus, server->cores);
        }
    }
    free(net_address);
    Network *ptr = realloc(group->nets, (group->size + 1) * sizeof(Network));
    if (!ptr) {
        return MEMORY_ERROR;
    }
    group->nets = ptr;
    if (init_network(&group->nets[group->size])) {
        return MEMORY_ERROR;
    }
    ++group->size;
    get_network_address(server, group->nets[group->size - 1].address);
    if (!init_server(&group->nets[group->size - 1].servers[0], server->dns, server->ip,
                     server->netmask, server->cpus, server->cores)) {
        ++group->nets[group->size - 1].size;
        ++group->nets[group->size - 1].mem_size;
        return SUCCESS;
    }
    free(group->nets[group->size - 1].servers);
    return NULLPTR_ERROR;
}

int print_by_networks(const Group *group) {
    if (!group) {
        printf("No group provided!\n");
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
               group->nets[i].address[2],
               group->nets[i].address[3]);
        for (unsigned j = 0; j < group->nets[i].size; ++j) {
            print_server(&group->nets[i].servers[j]);
        }
    }
    return SUCCESS;
}
