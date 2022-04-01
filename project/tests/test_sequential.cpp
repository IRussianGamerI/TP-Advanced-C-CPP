#include <gtest/gtest.h>

extern "C" {
    #include "common.h"
}

TEST(FindMaxDiff, WrongParams) {
    int *array = static_cast<int *>(malloc(sizeof(int)));
    time_diff_t result;
    EXPECT_EQ(find_max_diff(NULL, 10, NULL), NULLPTR_ERROR);
    EXPECT_EQ(find_max_diff(array, 10, NULL), NULLPTR_ERROR);
    EXPECT_EQ(find_max_diff(NULL, 10, &result), NULLPTR_ERROR);
    EXPECT_EQ(find_max_diff(array, 1, &result), WRONG_PARAMS);
    EXPECT_EQ(result.time, 0);
    EXPECT_EQ(result.diff, 0);
    free(array);
}

TEST(FindMaxDiff, CorrectParams) {
    int *array = static_cast<int *>(malloc(sizeof(int) * 10));
    ASSERT_TRUE(array);
    for (int i = 0; i < 10; ++i) {
        array[i] = 1 << i;
    }
    time_diff_t res;
    EXPECT_EQ(find_max_diff(array, 10, &res), SUCCESS);
    EXPECT_EQ(res.time, 9);
    EXPECT_EQ(res.diff, 256);
    for (int i = 0; i < 10; ++i) {
        array[i] = 10 - i;
    }
    EXPECT_EQ(find_max_diff(array, 10, &res), NOT_FOUND);
    free(array);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}