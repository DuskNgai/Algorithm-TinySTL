#include <random>

#include <gtest/gtest.h>

#include <Part 1/Chapter 4/MatrixMultiply.hpp>
#include <Utility/Utility.hpp>
#include <stl_vector.hpp>

constexpr int n = 8;

class TestMatrixMultiply : public testing::Test {
protected:
    Algorithm::SquareMatrix<int> a, b, c;

protected:
    virtual void SetUp() override {
        a = Algorithm::SquareMatrix<int>(n);
        b = Algorithm::SquareMatrix<int>(n);
        c = Algorithm::SquareMatrix<int>(n);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                a(i, j) = i * n + j;
            }
        }
        for (int i = 0; i < n; ++i) {
            b(i, n - i - 1) = 1;
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                c(i, j) = (i + 1) * n - (j + 1);
            }
        }
    }
};

TEST_F(TestMatrixMultiply, MatrixMultiply) {
    Algorithm::SquareMatrix<int> d(n);
    Algorithm::MatrixMultiply(a, b, d);
    EXPECT_PRED2(Algorithm::EqualTwoContainer<Algorithm::SquareMatrix<int>>, c, d);
}

TEST_F(TestMatrixMultiply, MatrixMultiplyRecursive) {
    Algorithm::SquareMatrix<int> d(n);
    Algorithm::MatrixMultiplyRecursive(a, b, d);
    EXPECT_PRED2(Algorithm::EqualTwoContainer<Algorithm::SquareMatrix<int>>, c, d);
}

TEST_F(TestMatrixMultiply, Strassen) {
    Algorithm::SquareMatrix<int> d(n);
    Algorithm::Strassen(a, b, d);
    EXPECT_PRED2(Algorithm::EqualTwoContainer<Algorithm::SquareMatrix<int>>, c, d);
}

TEST_F(TestMatrixMultiply, MatrixMultiplyRandom) {
    Algorithm::SquareMatrix<int> d(n), e(n), f(n), g(n);
    std::mt19937_64 rng;
    std::uniform_int_distribution<int> dist(-10, 10);
    TinySTL::generate_n(d.begin(), n * n, [&]() { return dist(rng); });

    Algorithm::MatrixMultiply(a, b, e);
    Algorithm::MatrixMultiplyRecursive(a, b, f);
    Algorithm::Strassen(a, b, g);
    EXPECT_PRED2(Algorithm::EqualTwoContainer<Algorithm::SquareMatrix<int>>, e, f);
    EXPECT_PRED2(Algorithm::EqualTwoContainer<Algorithm::SquareMatrix<int>>, e, g);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
