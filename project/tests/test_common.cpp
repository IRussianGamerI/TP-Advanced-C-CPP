#include <gtest/gtest.h>

extern "C" {
    #include "common.h"
}

constexpr const char *TEST_VEC_READ_FILENAME = "project/samples/default.tst";
constexpr unsigned EXPECTED_CAPACITY = 16;
constexpr unsigned EXPECTED_SIZE = 10;

TEST(CreateVector, NullptrParam) {
    EXPECT_EQ(create_vector(nullptr), NULLPTR_ERROR);
}

TEST(CreateVector, CorrectParam) {
    vector_t vector;
    ASSERT_EQ(create_vector(&vector), SUCCESS);
    free(vector.array);
}

TEST(PushBack, NullptrParam) {
    EXPECT_EQ(push_back(nullptr, 0), NULLPTR_ERROR);
    vector_t vec;
    vec.array = nullptr;
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

TEST(DeleteVector, NullptrParams) {
    EXPECT_EQ(delete_vector(nullptr), NULLPTR_ERROR);
    vector_t vec;
    vec.array = nullptr;
    EXPECT_EQ(delete_vector(&vec), NULLPTR_ERROR);
}

TEST(DeleteVector, CorrectParam) {
    vector_t vec;
    ASSERT_EQ(create_vector(&vec), SUCCESS);
    ASSERT_EQ(push_back(&vec, 1), SUCCESS);
    ASSERT_EQ(delete_vector(&vec), SUCCESS);
    EXPECT_EQ(vec.array, nullptr);
    EXPECT_EQ(vec.size, 0);
    EXPECT_EQ(vec.capacity, 0);
}

TEST(ReadVector, NullptrParams) {
    vector_t vec;
    EXPECT_EQ(read_vector(nullptr, nullptr), NULLPTR_ERROR);
    EXPECT_EQ(read_vector(&vec, nullptr), NULLPTR_ERROR);
    FILE *input = fopen(TEST_VEC_READ_FILENAME, "r");
    ASSERT_TRUE(input);
    EXPECT_EQ(read_vector(nullptr, input), NULLPTR_ERROR);
    fclose(input);
}

TEST(ReadVector, CorrectParams) {
    vector_t vec;
    ASSERT_EQ(create_vector(&vec), SUCCESS);
    FILE *input = fopen(TEST_VEC_READ_FILENAME, "r");
    ASSERT_TRUE(input);
    EXPECT_EQ(read_vector(&vec, input), SUCCESS);
    fclose(input);
    EXPECT_EQ(vec.capacity, EXPECTED_CAPACITY);
    EXPECT_EQ(vec.size, EXPECTED_SIZE);
    EXPECT_EQ(delete_vector(&vec), SUCCESS);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}