#include <gtest/gtest.h>

#include <stl_algorithm.hpp>
#include <stl_vector.hpp>

class TestRandom : public testing::Test {
protected:
    TinySTL::vector<int> list1;

protected:
    virtual void SetUp() override {
        {
            int data[] = {4, 2, 9, 1, 0};
            list1.insert(list1.end(), std::begin(data), std::end(data));
        }
    }
};

TEST_F(TestRandom, TestRandomShuffle) {
    TinySTL::random_shuffle(list1.begin(), list1.end());
    TinySTL::copy(list1.begin(), list1.end(), TinySTL::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
