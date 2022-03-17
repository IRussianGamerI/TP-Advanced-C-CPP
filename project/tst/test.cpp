#include <gtest/gtest.h>

extern "C" {
#include "server.h"
#include "group_of_networks.h"
}

TEST(INIT_SERVER_TEST, correct_data) {
    Server server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(&server, dns, ip, netmask, 4, 16), SUCCESS);
}

TEST(INIT_SERVER_TEST, nullptr_server) {
    Server *server = NULL;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(server, dns, ip, netmask, 4, 16), NULLPTR_ERROR);
}

TEST(INIT_SERVER_TEST, nullptr_ip_dns_netmask) {
    Server server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(&server, NULL, ip, netmask, 4, 16), NULLPTR_ERROR);
    EXPECT_EQ(init_server(&server, dns, NULL, netmask, 4, 16), NULLPTR_ERROR);
    EXPECT_EQ(init_server(&server, dns, ip, NULL, 4, 16), NULLPTR_ERROR);
}

TEST(INIT_SERVER_TEST, cpus_cores_negative_number) {
    Server server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(&server, dns, ip, netmask, -1, 16), INCORRECT_NUMBER);
    EXPECT_EQ(init_server(&server, dns, ip, netmask, 4, -1), INCORRECT_NUMBER);
}

TEST(STR_COUNT_TEST, correct_str) {
    EXPECT_EQ(str_count("192.168.1.1", '.'), 3);
    EXPECT_EQ(str_count("192.16811", '.'), 1);
    EXPECT_EQ(str_count("19216811", '.'), 0);
}

TEST(STR_COUNT_TEST, null_input) {
    EXPECT_EQ(str_count(NULL, '.'), 0);
    EXPECT_EQ(str_count("192.168.1.1", (char) 0), 0);
}

TEST(EXTRACT_IP_TEST, correct_ip) {
    uchar to[4];
    EXPECT_EQ(extract_ip("192.168.1.1", to), SUCCESS);
    EXPECT_EQ(to[0], 192);
    EXPECT_EQ(to[1], 168);
    EXPECT_EQ(to[2], 1);
    EXPECT_EQ(to[3], 1);
}

TEST(EXTRACT_IP_TEST, nullptr_params) {
    uchar to[4];
    EXPECT_EQ(extract_ip("192.168.1.1", NULL), NULLPTR_ERROR);
    EXPECT_EQ(extract_ip(NULL, to), NULLPTR_ERROR);
}

TEST(EXTRACT_IP_TEST, invalid_ip) {
    uchar to[4];
    EXPECT_EQ(extract_ip("1234", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("...", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("..", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("123.1.1.666", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("1234.1.1.66", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("66.1.1.1234", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("-1.-1.-1.-1", to), INCORRECT_INPUT);
}

TEST(READ_IP_TEST, nullptr_params) {
    uchar ip[4] = {124, 68, 222, 51};
    const char *msg = "Nothing here";
    EXPECT_EQ(read_ip(NULL, ip, msg), NULLPTR_ERROR);
    EXPECT_EQ(read_ip(stdin, NULL, msg), NULLPTR_ERROR);
    EXPECT_EQ(read_ip(stdin, NULL, NULL), NULLPTR_ERROR);
}

TEST(READ_IP_TEST, correct_params) {
    uchar ip[4] = {0};
    FILE *in = fmemopen((void *) "192.168.1.1", 12, "r");
    EXPECT_EQ(read_ip(in, ip, "Correct test"), SUCCESS);
}

TEST(COMPARE_IP_TEST, correct_ip_ptr) {
    uchar ip1[4] = {1, 2, 3, 4};
    uchar ip2[4] = {1, 2, 3, 4};
    EXPECT_EQ(compare_ip(ip1, ip2), true);
    ip2[3] = 5;
    EXPECT_EQ(compare_ip(ip1, ip2), false);
}

TEST(COMPARE_IP_TEST, nullptr_params) {
    uchar ip[4] = {1, 2, 3, 4};
    EXPECT_EQ(compare_ip(ip, NULL), 0);
    EXPECT_EQ(compare_ip(NULL, ip), 0);
}

TEST(PRINT_SERVER, correct_server) {
    Server server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    init_server(&server, dns, ip, netmask, 4, 16);
    EXPECT_EQ(print_server(&server), SUCCESS);
}

TEST(PRINT_SERVER, nullptr_server) {
    Server *not_a_server = NULL;
    EXPECT_EQ(print_server(not_a_server), NULLPTR_ERROR);
}

// NETWORK TESTS BELOW

TEST(INIT_NETWORK, init_test) {
    EXPECT_EQ(init_network(NULL), NULLPTR_ERROR);
    Network net;
    EXPECT_EQ(init_network(&net), SUCCESS);
    EXPECT_EQ(destroy_network(&net), SUCCESS);
}

TEST(DESTROY_NETWORK, nullptr_case) {
    EXPECT_EQ(destroy_network(NULL), NULLPTR_ERROR);
}

TEST(INIT_GROUP, init_test) {
    EXPECT_EQ(init_group(NULL), NULLPTR_ERROR);

    Group group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    EXPECT_EQ(group.size, 0);
    EXPECT_FALSE(group.nets);
}

TEST(DESTROY_GROUP, group_correct_and_nullptr) {
    Group group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    EXPECT_EQ(destroy_group(&group), SUCCESS);
    EXPECT_EQ(destroy_group(NULL), NULLPTR_ERROR);
}

TEST(ADD_TO_GROUP, every_case) {
    Group group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    Server server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    ASSERT_EQ(init_server(&server, dns, ip, netmask, 6, 36), SUCCESS);

    EXPECT_EQ(add_to_network(NULL, &server), NULLPTR_ERROR);
    EXPECT_EQ(add_to_network(&group, NULL), NULLPTR_ERROR);

    ASSERT_EQ(add_to_network(&group, &server), SUCCESS);
    EXPECT_EQ(group.size, 1);
    EXPECT_EQ(group.nets[0].size, 1);
    
    ip[3] = 224;
    ASSERT_EQ(init_server(&server, dns, ip, netmask, 2, 20), SUCCESS);
    ASSERT_EQ(add_to_network(&group, &server), SUCCESS);
    EXPECT_EQ(group.nets[0].size, 2);
    EXPECT_EQ(group.size, 1);
    
    for (ip[3] = 223; ip[3] > 220; --ip[3]) {
        ASSERT_EQ(init_server(&server, dns, ip, netmask, 2, 20), SUCCESS);
        ASSERT_EQ(add_to_network(&group, &server), SUCCESS);
    }
    EXPECT_EQ(destroy_group(&group), SUCCESS);
}

TEST(PRINT_BY_NETWORKS, correct) {
    Group group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    Server server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    ASSERT_EQ(init_server(&server, dns, ip, netmask, 6, 36), SUCCESS);
    for (ip[3] = 223; ip[3] > 220; --ip[3]) {
        ASSERT_EQ(init_server(&server, dns, ip, netmask, 2, 20), SUCCESS);
        ASSERT_EQ(add_to_network(&group, &server), SUCCESS);
    }

    EXPECT_EQ(print_by_networks(&group), SUCCESS);
    
    EXPECT_EQ(destroy_group(&group), SUCCESS);
}

TEST(PRINT_BY_NETWORKS, nullptr_test) {
    Group group;
    EXPECT_EQ(print_by_networks(NULL), NULLPTR_ERROR);
    ASSERT_EQ(init_group(&group), SUCCESS);
    EXPECT_EQ(print_by_networks(&group), NULLPTR_ERROR);
    group.nets = static_cast<Network *>(calloc(1, sizeof(Network)));
    group.size = 1;
    EXPECT_EQ(print_by_networks(&group), NULLPTR_ERROR);
    free(group.nets);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}