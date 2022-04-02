#include <gtest/gtest.h>

extern "C" {
    #include "common.h"
    #include "parallel.h"
}

constexpr const char *DEFAULT_SAMPLE_FILENAME = "../project/samples/default.tst";

TEST(AllocPipes, WrongParam) {
    ASSERT_FALSE(alloc_pipes(0));
}

TEST(AllocPipes, CorrectParam) {
    int amount = sysconf(_SC_NPROCESSORS_ONLN);
    int **pipes = alloc_pipes(amount);
    EXPECT_TRUE(pipes);
    for (int i = 0; i < amount; ++i) {
        free(pipes[i]);
    }
    free(pipes);
}

TEST(FindMaxDiffOnSection, WrongParams) {
    EXPECT_EQ(find_max_diff_on_section(nullptr, 0, 10, nullptr), NULLPTR_ERROR);
    int *array = static_cast<int *>(calloc(sizeof(int), 10));
    ASSERT_TRUE(array);
    EXPECT_EQ(find_max_diff_on_section(array, 0, 10, nullptr), NULLPTR_ERROR);
    time_diff_t result;
    EXPECT_EQ(find_max_diff_on_section(nullptr, 0, 10, &result), NULLPTR_ERROR);
    EXPECT_EQ(find_max_diff_on_section(array, 0, 1, &result), WRONG_PARAMS);
    free(array);
}

TEST(FindMaxDiffOnSection, CorrectParams) {
    vector_t vec;
    FILE *input = fopen(DEFAULT_SAMPLE_FILENAME, "r");
    ASSERT_TRUE(input);
    ASSERT_EQ(create_vector(&vec), SUCCESS);
    EXPECT_EQ(read_vector(&vec, input), SUCCESS);
    fclose(input);
    time_diff_t result;
    EXPECT_EQ(find_max_diff_on_section(vec.array, 0, vec.size / 2 + 1, &result), SUCCESS);
    EXPECT_EQ(result.diff, 1);
    EXPECT_EQ(result.time, 1);
    EXPECT_EQ(find_max_diff_on_section(vec.array, vec.size / 2, vec.size, &result), SUCCESS);
    EXPECT_EQ(result.diff, 2);
    EXPECT_EQ(result.time, 9);
    EXPECT_EQ(delete_vector(&vec), SUCCESS);
    int sample[] = {5, 4, 3, 2, 1};
    EXPECT_EQ(find_max_diff_on_section(sample, 0, 5, &result), NOT_FOUND);
}

TEST(FindMaxFromPipes, WrongParams) {
    EXPECT_EQ(find_max_from_pipes(nullptr, 1, nullptr), NULLPTR_ERROR);
    int **pipes = static_cast<int **>(calloc(sizeof(int *), 6));
    time_diff_t result;
    EXPECT_EQ(find_max_from_pipes(pipes, 0, &result), WRONG_PARAMS);
    EXPECT_EQ(find_max_from_pipes(pipes, 6, &result), NULLPTR_ERROR);
    free(pipes);
}

TEST(FindMaxDiff, WrongParams) {
    int *array = static_cast<int *>(calloc(10, sizeof(int)));
    time_diff_t result;
    EXPECT_EQ(find_max_diff(array, 10, nullptr), NULLPTR_ERROR);
    EXPECT_EQ(find_max_diff(nullptr, 10, &result), NULLPTR_ERROR);
    EXPECT_EQ(find_max_diff(array, 1, &result), WRONG_PARAMS);
    free(array);
}

TEST(FindMaxDiff, CorrectParams) {
    vector_t vec;
    EXPECT_EQ(create_vector(&vec), SUCCESS);
    FILE *input = fopen(DEFAULT_SAMPLE_FILENAME, "r");
    if (!input) {
        delete_vector(&vec);
        return;
    }
    EXPECT_EQ(read_vector(&vec, input), SUCCESS);
    fclose(input);
    time_diff_t result;
    EXPECT_EQ(find_max_diff(vec.array, vec.size, &result), SUCCESS);
    EXPECT_EQ(result.diff, 2);
    EXPECT_EQ(result.time, 9);
    EXPECT_EQ(delete_vector(&vec), SUCCESS);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}