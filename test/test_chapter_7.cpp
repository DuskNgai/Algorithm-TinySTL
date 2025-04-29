#include <gtest/gtest.h>

#include <Part 2/Chapter 7/QuickSort.hpp>
#include <random>
#include <stl_algorithm.hpp>
#include <stl_function.hpp>
#include <stl_vector.hpp>

class TestQuickSort : public ::testing::Test {
protected:
    TinySTL::vector<int> repeat, repeat_copy;
    TinySTL::vector<int> empty, empty_copy;
    TinySTL::vector<int> negative, negative_copy;
    TinySTL::vector<double> fp, fp_copy;
    TinySTL::vector<int> sorted, sorted_copy;
    TinySTL::vector<int> reversed, reversed_copy;
    TinySTL::vector<int> large, large_copy;

protected:
    virtual void SetUp() override {
        {
            // Repeating terms.
            int data[]{ 11, 1, 51, 1, 5, 3 };
            repeat = repeat_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Empty list should return empty list.
            empty = empty_copy;
        }
        {
            // Negative number.
            int data[]{ 1, 1, -5, 6 };
            negative = negative_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Floating point.
            double data[]{ 11, -4, 20, 15, 13.5, -20 };
            fp = fp_copy = TinySTL::vector<double>(std::begin(data), std::end(data));
        }
        {
            // Already sorted array.
            int data[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            sorted = sorted_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Array in reversed sorted order.
            int data[]{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
            reversed = reversed_copy = TinySTL::vector<int>(std::begin(data), std::end(data));
        }
        {
            // Large array.
            std::mt19937_64 rng(0);
            std::uniform_int_distribution<int> dist(-5000, 5000);
            large.reserve(1000);
            TinySTL::generate_n(TinySTL::back_inserter(large), 1000, [&]() {
                return dist(rng);
            });
            large_copy.insert(large_copy.end(), large.begin(), large.end());
        }
    }
};

TEST_F(TestQuickSort, QuickSortRepeatingTerms) {
    TinySTL::sort(repeat.begin(), repeat.end());
    Algorithm::quick_sort(repeat_copy.begin(), repeat_copy.end(), TinySTL::less<int>());
    EXPECT_EQ(repeat, repeat_copy);
}

TEST_F(TestQuickSort, QuickSortEmptyList) {
    TinySTL::sort(empty.begin(), empty.end());
    Algorithm::quick_sort(empty_copy.begin(), empty_copy.end(), TinySTL::less<int>());
    EXPECT_EQ(empty, empty_copy);
}

TEST_F(TestQuickSort, QuickSortNegativeNumber) {
    TinySTL::sort(negative.begin(), negative.end());
    Algorithm::quick_sort(negative_copy.begin(), negative_copy.end(), TinySTL::less<int>());
    EXPECT_EQ(negative, negative_copy);
}

TEST_F(TestQuickSort, QuickSortFloatingPoint) {
    TinySTL::sort(fp.begin(), fp.end());
    Algorithm::quick_sort(fp_copy.begin(), fp_copy.end(), TinySTL::less<int>());
    EXPECT_EQ(fp, fp_copy);
}

TEST_F(TestQuickSort, QuickSortSortedArray) {
    TinySTL::sort(sorted.begin(), sorted.end());
    Algorithm::quick_sort(sorted_copy.begin(), sorted_copy.end(), TinySTL::less<int>());
    EXPECT_EQ(sorted, sorted_copy);
}

TEST_F(TestQuickSort, QuickSortReversedArray) {
    TinySTL::sort(reversed.begin(), reversed.end());
    Algorithm::quick_sort(reversed_copy.begin(), reversed_copy.end(), TinySTL::less<int>());
    EXPECT_EQ(reversed, reversed_copy);
}

TEST_F(TestQuickSort, QuickSortLargeArray) {
    TinySTL::sort(large.begin(), large.end());
    Algorithm::quick_sort(large_copy.begin(), large_copy.end(), TinySTL::less<int>());
    EXPECT_EQ(large, large_copy);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
