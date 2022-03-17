#include <stdio.h>
#include "server.h"
#include "group_of_networks.h"

int main() {
    ssize_t n;
    fprintf(stdout, "Enter the amount of servers: ");
    if (fscanf(stdin, "%zd", &n) != 1) {
        fprintf(stdout, "You entered incorrect number\n");
        return INCORRECT_INPUT;
    }
    Server current;  // Сервер, только что введенный с клавиатуры
    Group group;
    init_group(&group);  // Инициализация пустой группы
    for (ssize_t i = 0; i < n; ++i) {
        if (read_ip(stdin, current.dns, "DNS") != SUCCESS) {
            return INPUT_ERROR;
        }

        if (read_ip(stdin, current.ip, "IP") != SUCCESS) {
            return INPUT_ERROR;
        }

        if (read_ip(stdin, current.netmask, "Netmask") != SUCCESS) {
            return INPUT_ERROR;
        }

        fprintf(stdout, "Enter the amount of CPUs: ");
        if (fscanf(stdin, "%d", &current.cpus) != 1) {
            return INPUT_ERROR;
        }

        fprintf(stdout, "Enter the amount of cores: ");
        if (fscanf(stdin, "%d", &current.cores) != 1) {
            return INPUT_ERROR;
        }

        if (add_to_network(&group, &current) != SUCCESS) {
            return MEMORY_ERROR;
        }
    }

    if (print_by_networks(&group) != SUCCESS) {
        return NULLPTR_ERROR;
    }
    destroy_group(&group);
    return 0;
}
