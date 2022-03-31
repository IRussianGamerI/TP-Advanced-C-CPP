#include <gtest/gtest.h>

extern "C" {
    #include "common.h"
}

TEST(CreateVector, NullptrParam) {
    EXPECT_EQ(create_vector(NULL), NULLPTR_ERROR);
}

TEST(CreateVector, CorrectParam) {
    vector_t vector;
    ASSERT_EQ(create_vector(&vector), SUCCESS);
    free(vector.array);
}

TEST(PushBack, NullptrParam) {
    EXPECT_EQ(push_back(NULL, 0), NULLPTR_ERROR);
    vector_t vec;
    vec.array = NULL;
    EXPECT_EQ(push_back(&vec, 0), NULLPTR_ERROR);
}

TEST(PushBack, CorrectParam) {
    vector_t vec;
    EXPECT_EQ(create_vector(&vec), SUCCESS);
    EXPECT_EQ(push_back(&vec, 0), SUCCESS);
    EXPECT_EQ(vec.size, 1);
    EXPECT_EQ(vec.capacity, 1);
    EXPECT_EQ(push_back(&vec, 1), SUCCESS);
    EXPECT_EQ(vec.size, 2);
    EXPECT_EQ(vec.capacity, 2);
    EXPECT_EQ(push_back(&vec, 2), SUCCESS);
    EXPECT_EQ(vec.size, 3);
    EXPECT_EQ(vec.capacity, 4);
    delete_vector(&vec);
}

// More tests will be implemented soon
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}