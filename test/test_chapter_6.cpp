#include <gtest/gtest.h>

#include <random>
#include <stl_heap.hpp>
#include <stl_queue.hpp>
#include <stl_vector.hpp>

class TestHeap : public testing::Test {
protected:
    TinySTL::vector<int> list1;
    TinySTL::vector<int> list2;
    TinySTL::vector<int> list3;
    TinySTL::vector<int> list4;

    TinySTL::vector<const char*> list5;

    std::mt19937_64 rng;

protected:
    virtual void SetUp() override {
        rng.seed(0);
        {
            int data[] = { 16, 4, 10, 14, 7, 9, 3, 2, 8, 1 };
            list1.insert(list1.end(), std::begin(data), std::end(data));
        }
        {
            int data[] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
            list2.insert(list2.end(), std::begin(data), std::end(data));
        }
        {
            int data[] = { 2, 14, 8, 4, 11, 9, 15, 16, 10, 17 };
            list3.insert(list3.end(), std::begin(data), std::end(data));
        }
        {
            int data[] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
            list4.insert(list4.end(), std::begin(data), std::end(data));
        }
        {
            const char* data[] = { "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "HI", "NH", "NY" };
            list5.insert(list5.end(), std::begin(data), std::end(data));
        }
    }
};

TEST_F(TestHeap, TestMaxHeap) {
    int data[] = { 16, 14, 10, 8, 7, 9, 3, 2, 4, 1 };
    TinySTL::vector<int> reference(std::begin(data), std::end(data));

    TinySTL::__adjust_heap(list1.begin(), (int64_t)1, (int64_t)list1.size(), list1[1]);
    EXPECT_TRUE(TinySTL::equal(list1.begin(), list1.end(), reference.begin()));

    TinySTL::make_heap(list2.begin(), list2.end());
    EXPECT_TRUE(TinySTL::equal(list2.begin(), list2.end(), reference.begin()));

    auto comp = [](const char* a, const char* b) {
        return strcmp(a, b) < 0;
    };
    TinySTL::random_shuffle(list5.begin(), list5.end(), rng);
    TinySTL::make_heap(list5.begin(), list5.end(), comp);
    TinySTL::copy(list5.begin(), list5.end(), TinySTL::ostream_iterator<const char*>(std::cout, " "));
    std::cout << std::endl;
    EXPECT_TRUE(TinySTL::is_heap(list5.begin(), list5.end(), comp));
}

TEST_F(TestHeap, TestMinHeap) {
    int data1[] = { 2, 4, 8, 10, 11, 9, 15, 16, 14, 17 };
    TinySTL::vector<int> reference1(std::begin(data1), std::end(data1));
    int data2[] = { 1, 2, 3, 4, 7, 9, 10, 14, 8, 16 };
    TinySTL::vector<int> reference2(std::begin(data2), std::end(data2));

    TinySTL::__adjust_heap(list3.begin(), (int64_t)1, (int64_t)list3.size(), list3[1], TinySTL::greater<int>());
    EXPECT_TRUE(TinySTL::equal(list3.begin(), list3.end(), reference1.begin()));

    TinySTL::make_heap(list4.begin(), list4.end(), TinySTL::greater<int>());
    EXPECT_TRUE(TinySTL::equal(list4.begin(), list4.end(), reference2.begin()));

    auto comp = [](const char* a, const char* b) {
        return strcmp(a, b) > 0;
    };
    TinySTL::random_shuffle(list5.begin(), list5.end(), rng);
    TinySTL::make_heap(list5.begin(), list5.end(), comp);
    TinySTL::copy(list5.begin(), list5.end(), TinySTL::ostream_iterator<const char*>(std::cout, " "));
    std::cout << std::endl;
    EXPECT_TRUE(TinySTL::is_heap(list5.begin(), list5.end(), comp));
}

TEST_F(TestHeap, TestMaxPriorityQueue) {
    auto comp = [](const char* a, const char* b) {
        return strcmp(a, b) < 0;
    };
    TinySTL::priority_queue<const char*, TinySTL::vector<const char*>, decltype(comp)> pq(list5.begin(), list5.end(), comp);
    EXPECT_STREQ(pq.top(), "NY");
}

TEST_F(TestHeap, TestMinPriorityQueue) {
    auto comp = [](const char* a, const char* b) {
        return strcmp(a, b) > 0;
    };
    TinySTL::priority_queue<const char*, TinySTL::vector<const char*>, decltype(comp)> pq(list5.begin(), list5.end(), comp);
    EXPECT_STREQ(pq.top(), "AK");
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
