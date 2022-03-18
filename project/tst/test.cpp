#include <gtest/gtest.h>

extern "C" {
#include "server.h"
#include "group_of_networks.h"
}

TEST(InitServerTest, CorrectData) {
    server_t server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(&server, dns, ip, netmask, 4, 16), SUCCESS);
}

TEST(InitServerTest, NullptrServer) {
    server_t *server = NULL;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(server, dns, ip, netmask, 4, 16), NULLPTR_ERROR);
}

TEST(InitServerTest, NullptrIpDnsNetmask) {
    server_t server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(&server, NULL, ip, netmask, 4, 16), NULLPTR_ERROR);
    EXPECT_EQ(init_server(&server, dns, NULL, netmask, 4, 16), NULLPTR_ERROR);
    EXPECT_EQ(init_server(&server, dns, ip, NULL, 4, 16), NULLPTR_ERROR);
}

TEST(InitServerTest, CpusCoresNegativeNumber) {
    server_t server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    EXPECT_EQ(init_server(&server, dns, ip, netmask, -1, 16), INCORRECT_NUMBER);
    EXPECT_EQ(init_server(&server, dns, ip, netmask, 4, -1), INCORRECT_NUMBER);
}

TEST(StrCountTest, CorrectStr) {
    EXPECT_EQ(str_count("192.168.1.1", '.'), 3);
    EXPECT_EQ(str_count("192.16811", '.'), 1);
    EXPECT_EQ(str_count("19216811", '.'), 0);
}

TEST(StrCountTest, NullInput) {
    EXPECT_EQ(str_count(NULL, '.'), 0);
    EXPECT_EQ(str_count("192.168.1.1", (char) 0), 0);
}

TEST(ExtractIpTest, CorrectIp) {
    uchar to[4];
    EXPECT_EQ(extract_ip("192.168.1.1", to), SUCCESS);
    EXPECT_EQ(to[0], 192);
    EXPECT_EQ(to[1], 168);
    EXPECT_EQ(to[2], 1);
    EXPECT_EQ(to[3], 1);
}

TEST(ExtractIpTest, NullptrParams) {
    uchar to[4];
    EXPECT_EQ(extract_ip("192.168.1.1", NULL), NULLPTR_ERROR);
    EXPECT_EQ(extract_ip(NULL, to), NULLPTR_ERROR);
}

TEST(ExtractIpTest, InvalidIp) {
    uchar to[4];
    EXPECT_EQ(extract_ip("1234", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("...", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("..", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("123.1.1.666", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("1234.1.1.66", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("66.1.1.1234", to), INCORRECT_INPUT);
    EXPECT_EQ(extract_ip("-1.-1.-1.-1", to), INCORRECT_INPUT);
}

TEST(ReadIpTest, NullptrParams) {
    uchar ip[4] = {124, 68, 222, 51};
    const char *msg = "Nothing here";
    EXPECT_EQ(read_ip(NULL, ip, msg), NULLPTR_ERROR);
    EXPECT_EQ(read_ip(stdin, NULL, msg), NULLPTR_ERROR);
    EXPECT_EQ(read_ip(stdin, NULL, NULL), NULLPTR_ERROR);
}

TEST(ReadIpTest, CorrectParams) {
    uchar ip[4] = {0};
    FILE *in = fmemopen((void *) "192.168.1.1", 12, "r");
    EXPECT_EQ(read_ip(in, ip, "Correct test"), SUCCESS);
}

TEST(CompareIpTest, CorrectIpPtr) {
    uchar ip1[4] = {1, 2, 3, 4};
    uchar ip2[4] = {1, 2, 3, 4};
    EXPECT_EQ(compare_ip(ip1, ip2), true);
    ip2[3] = 5;
    EXPECT_EQ(compare_ip(ip1, ip2), false);
}

TEST(CompareIpTest, NullptrParams) {
    uchar ip[4] = {1, 2, 3, 4};
    EXPECT_EQ(compare_ip(ip, NULL), 0);
    EXPECT_EQ(compare_ip(NULL, ip), 0);
}

TEST(PrintServer, CorrectServer) {
    server_t server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    init_server(&server, dns, ip, netmask, 4, 16);
    EXPECT_EQ(print_server(&server), SUCCESS);
}

TEST(ReadServer, Everything) {
    server_t server;
    FILE *in = fmemopen((void *) "", 0, "r");
    EXPECT_EQ(read_server(in, &server), INPUT_ERROR);
    fclose(in);
    in = fmemopen((void *) "192.168.1.1", strlen("192.168.1.1"), "r");
    EXPECT_EQ(read_server(in, &server), INPUT_ERROR);
    fclose(in);
    in = fmemopen((void *) "192.168.1.1\n122.123.1.1", strlen("192.168.1.1\n122.123.1.1"), "r");
    EXPECT_EQ(read_server(in, &server), INPUT_ERROR);
    fclose(in);
    in = fmemopen((void *) "192.168.1.1\n122.123.1.1\n1.11.1.1",
                  strlen("192.168.1.1\n122.123.1.1\n1.11.1.1"), "r");
    EXPECT_EQ(read_server(in, &server), INPUT_ERROR);
    fclose(in);
    in = fmemopen((void *) "192.168.1.1\n122.123.1.1\n1.11.1.1\n4",
                  strlen("192.168.1.1\n122.123.1.1\n1.11.1.1\n4"), "r");
    EXPECT_EQ(read_server(in, &server), INPUT_ERROR);
    fclose(in);
    in = fmemopen((void *) "192.168.1.1\n122.123.1.1\n1.11.1.1\n4 20",
                  strlen("192.168.1.1\n122.123.1.1\n1.11.1.1\n 4 20"), "r");
    EXPECT_EQ(read_server(in, &server), SUCCESS);
    fclose(in);
}

TEST(PrintServer, NullptrServer) {
    server_t *not_a_server = NULL;
    EXPECT_EQ(print_server(not_a_server), NULLPTR_ERROR);
}

// NETWORK TESTS BELOW

TEST(InitNetwork, InitTest) {
    EXPECT_EQ(init_network(NULL), NULLPTR_ERROR);
    network_t net;
    EXPECT_EQ(init_network(&net), SUCCESS);
    EXPECT_EQ(destroy_network(&net), SUCCESS);
}

TEST(DestroyNetwork, NullptrCase) {
    EXPECT_EQ(destroy_network(NULL), NULLPTR_ERROR);
}

TEST(AddToNetwork, NullptrCases) {
    network_t network;
    server_t server;
    EXPECT_EQ(add_to_network(NULL, &server), NULLPTR_ERROR);
    EXPECT_EQ(add_to_network(&network, NULL), NULLPTR_ERROR);
    network.servers = NULL;
    EXPECT_EQ(add_to_network(&network, &server), NULLPTR_ERROR);
}

TEST(InitGroup, InitTest) {
    EXPECT_EQ(init_group(NULL), NULLPTR_ERROR);

    group_t group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    EXPECT_EQ(group.size, 0);
    EXPECT_FALSE(group.nets);
}

TEST(DestroyGroup, GroupCorrectAndNullptr) {
    group_t group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    EXPECT_EQ(destroy_group(&group), SUCCESS);
    EXPECT_EQ(destroy_group(NULL), NULLPTR_ERROR);
}

TEST(AddToGroup, EveryCase) {
    group_t group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    server_t server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    ASSERT_EQ(init_server(&server, dns, ip, netmask, 6, 36), SUCCESS);

    EXPECT_EQ(add_to_group(NULL, &server), NULLPTR_ERROR);
    EXPECT_EQ(add_to_group(&group, NULL), NULLPTR_ERROR);

    ASSERT_EQ(add_to_group(&group, &server), SUCCESS);
    EXPECT_EQ(group.size, 1);
    EXPECT_EQ(group.nets[0].size, 1);

    ip[3] = 224;
    ASSERT_EQ(init_server(&server, dns, ip, netmask, 2, 20), SUCCESS);
    ASSERT_EQ(add_to_group(&group, &server), SUCCESS);
    EXPECT_EQ(group.nets[0].size, 2);
    EXPECT_EQ(group.size, 1);

    for (ip[3] = 223; ip[3] > 220; --ip[3]) {
        ASSERT_EQ(init_server(&server, dns, ip, netmask, 2, 20), SUCCESS);
        ASSERT_EQ(add_to_group(&group, &server), SUCCESS);
    }
    EXPECT_EQ(destroy_group(&group), SUCCESS);
}

TEST(PrintByNetworks, Correct) {
    group_t group;
    ASSERT_EQ(init_group(&group), SUCCESS);
    server_t server;
    uchar dns[4] = {8, 8, 4, 4};
    uchar ip[4] = {124, 68, 222, 51};
    uchar netmask[4] = {255, 255, 255, 0};
    ASSERT_EQ(init_server(&server, dns, ip, netmask, 6, 36), SUCCESS);
    for (ip[3] = 223; ip[3] > 220; --ip[3]) {
        ASSERT_EQ(init_server(&server, dns, ip, netmask, 2, 20), SUCCESS);
        ASSERT_EQ(add_to_group(&group, &server), SUCCESS);
    }

    EXPECT_EQ(print_by_networks(&group), SUCCESS);

    EXPECT_EQ(destroy_group(&group), SUCCESS);
}

TEST(PrintByNetworks, NullptrTest) {
    group_t group;
    EXPECT_EQ(print_by_networks(NULL), NULLPTR_ERROR);
    ASSERT_EQ(init_group(&group), SUCCESS);
    EXPECT_EQ(print_by_networks(&group), NULLPTR_ERROR);
    group.nets = static_cast<network_t *>(calloc(1, sizeof(network_t)));
    group.size = 1;
    EXPECT_EQ(print_by_networks(&group), NULLPTR_ERROR);
    free(group.nets);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}