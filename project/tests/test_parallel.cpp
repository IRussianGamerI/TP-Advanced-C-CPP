#include <gtest/gtest.h>

extern "C" {
    #include "common.h"
    #include "parallel.h"
}

TEST(AllocPipes, WrongParam) {
    ASSERT_FALSE(alloc_pipes(0));
}

// Tests will be implemented soon

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}