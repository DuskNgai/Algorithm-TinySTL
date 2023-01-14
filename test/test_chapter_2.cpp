#include <random>

#include <gtest/gtest.h>

#include <Part 1/Chapter 2/InsertionSort.hpp>
#include <Part 1/Chapter 2/MergeSort.hpp>
#include <Utility/Utility.hpp>
#include <stl_vector.hpp>

class TestSort : public testing::Test {
protected:
    TinySTL::vector<int> repeat, repeat_copy;
    TinySTL::vector<int> empty, empty_copy;
    TinySTL::vector<int> negative, negative_copy;
    TinySTL::vector<double> fp, fp_copy;
    TinySTL::vector<int> sorted, sorted_copy;
    TinySTL::vector<int> reversed, reversed_copy;
    TinySTL::vector<int> large, large_copy;

protected:
    virtual void SetUp() {
        {
            // Repeating terms.
            int data[] = {11, 1, 51, 1, 5, 3};
            repeat = repeat_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Empty list should return empty list.
            empty = empty_copy;
        }
        {
            // Negative number.
            int data[] = {1, 1, -5, 6};
            negative = negative_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Floating point.
            double data[] = {11, -4, 20, 15, 13.5, -20};
            fp = fp_copy = TinySTL::vector<double>(std::begin(data), std::end(data));
        }
        {
            // Already sorted array.
            int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            sorted = sorted_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Array in reversed sorted order.
            int data[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
            reversed = reversed_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Large array.
            std::mt19937_64 rng;
            std::uniform_int_distribution<int> dist(-5000, 5000);
            large.reserve(1000);
            std::generate_n(std::back_inserter(large), 1000, [&](){ return dist(rng); });
            large_copy.insert(large_copy.end(), large.begin(), large.end());
        }
    }
};

TEST_F(TestSort, InsertionSortRepeatingTerms) {
    TinySTL::sort(repeat.begin(), repeat.end(), std::less<int>());
    Algorithm::InsertionSort(repeat_copy.begin(), repeat_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, repeat, repeat_copy);
}

TEST_F(TestSort, InsertionSortEmptyList) {
    TinySTL::sort(empty.begin(), empty.end(), std::less<int>());
    Algorithm::InsertionSort(empty_copy.begin(), empty_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, empty, empty_copy);
}

TEST_F(TestSort, InsertionSortNegativeNumber) {
    TinySTL::sort(negative.begin(), negative.end(), std::less<int>());
    Algorithm::InsertionSort(negative_copy.begin(), negative_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, negative, negative_copy);
}

TEST_F(TestSort, InsertionSortFloatingPoint) {
    TinySTL::sort(fp.begin(), fp.end(), std::less<double>());
    Algorithm::InsertionSort(fp_copy.begin(), fp_copy.end(), std::less<double>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<double>>, fp, fp_copy);
}

TEST_F(TestSort, InsertionSortSortedArray) {
    TinySTL::sort(sorted.begin(), sorted.end(), std::less<int>());
    Algorithm::InsertionSort(sorted_copy.begin(), sorted_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, sorted, sorted_copy);
}

TEST_F(TestSort, InsertionSortReversedArray) {
    TinySTL::sort(reversed.begin(), reversed.end(), std::less<int>());
    Algorithm::InsertionSort(reversed_copy.begin(), reversed_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, reversed, reversed_copy);
}

TEST_F(TestSort, InsertionSortLargeArray) {
    TinySTL::sort(large.begin(), large.end(), std::less<int>());
    Algorithm::InsertionSort(large_copy.begin(), large_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, large, large_copy);
}

TEST_F(TestSort, MergeSortRepeatingTerms) {
    TinySTL::sort(repeat.begin(), repeat.end(), std::less<int>());
    Algorithm::MergeSort(repeat_copy.begin(), repeat_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, repeat, repeat_copy);
}

TEST_F(TestSort, MergeSortEmptyList) {
    TinySTL::sort(empty.begin(), empty.end(), std::less<int>());
    Algorithm::MergeSort(empty_copy.begin(), empty_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, empty, empty_copy);
}

TEST_F(TestSort, MergeSortNegativeNumber) {
    TinySTL::sort(negative.begin(), negative.end(), std::less<int>());
    Algorithm::MergeSort(negative_copy.begin(), negative_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, negative, negative_copy);
}

TEST_F(TestSort, MergeSortFloatingPoint) {
    TinySTL::sort(fp.begin(), fp.end(), std::less<double>());
    Algorithm::MergeSort(fp_copy.begin(), fp_copy.end(), std::less<double>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<double>>, fp, fp_copy);
}

TEST_F(TestSort, MergeSortSortedArray) {
    TinySTL::sort(sorted.begin(), sorted.end(), std::less<int>());
    Algorithm::MergeSort(sorted_copy.begin(), sorted_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, sorted, sorted_copy);
}

TEST_F(TestSort, MergeSortReversedArray) {
    TinySTL::sort(reversed.begin(), reversed.end(), std::less<int>());
    Algorithm::MergeSort(reversed_copy.begin(), reversed_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, reversed, reversed_copy);
}

TEST_F(TestSort, MergeSortLargeArray) {
    TinySTL::sort(large.begin(), large.end(), std::less<int>());
    Algorithm::MergeSort(large_copy.begin(), large_copy.end(), std::less<int>());
    EXPECT_PRED2(Algorithm::EqualTwoContainer<TinySTL::vector<int>>, large, large_copy);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
