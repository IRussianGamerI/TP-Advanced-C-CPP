#include <stdio.h>

#include "group_of_networks.h"
#include "server.h"

int main() {
    ssize_t n;
    printf("Enter the amount of servers: ");
    if (scanf("%zd", &n) != 1) {
        printf("You entered incorrect number\n");
        return INCORRECT_INPUT;
    }
    server_t current;  // Сервер, только что введенный с клавиатуры
    group_t group;
    init_group(&group);  // Инициализация пустой группы
    for (ssize_t i = 0; i < n; ++i) {
        read_server(stdin, &current);
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
