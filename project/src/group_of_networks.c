#include "group_of_networks.h"
#include <malloc.h>
#include "server.h"

void init_group(Group *group) {
    group->size = 0;
    group->nets = NULL;
}

void destroy_group(Group *group) {
    if (!group->nets) {
        return;
    }
    for (unsigned i = 0; i < group->size; ++i) {
        free(group->nets[i].servers);
    }
    free(group->nets);
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
                void *ptr = realloc(group->nets[i].servers, group->nets[i].mem_size * 2);
                if (!ptr) {
                    return MEMORY_ERROR;
                }
                group->nets[i].servers = ptr;
                group->nets[i].mem_size *= 2;
            }
            ++group->nets[i].size;
            return init_server(&group->nets[i].servers[group->nets[i].size - 1], server->dns, server->ip,
                               server->netmask, server->cpus, server->cores);
        }
    }
    void *ptr = realloc(group->nets, ++group->size);
    if (!ptr) {
        return MEMORY_ERROR;
    }
    group->nets = ptr;
    if (!(group->nets[group->size - 1].servers = malloc(sizeof(Server)))) {
        return MEMORY_ERROR;
    }
    get_network_address(server, group->nets[group->size - 1].address);
    if (!init_server(&group->nets[group->size - 1].servers[0], server->dns, server->ip,
                     server->netmask, server->cpus, server->cores)) {
        ++group->nets[group->size - 1].size;
        return SUCCESS;
    }
    return NULLPTR_ERROR;
}

int print_by_networks(const Group *group) {
    if (!group) {
        printf("No group provided!\n");
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
        for (unsigned j = 0; j < group->nets[i].size; ++i) {
            print_server(&group->nets[i].servers[j]);
        }
    }
    return SUCCESS;
}