#ifndef _ALGORITHM_MATRIX_MULTIPLY_HPP_
#define _ALGORITHM_MATRIX_MULTIPLY_HPP_

#include <stl_vector.hpp>

namespace Algorithm {

    template <typename T>
    class SquareMatrix {
    private:
        int m_n;
        TinySTL::vector<T> m_data;

    public:
        SquareMatrix() = default;
        SquareMatrix(int n)
            : m_n(n)
            , m_data(n * n) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    this->operator()(i, j) = T();
                }
            }
        }
        ~SquareMatrix() = default;

    public:
        SquareMatrix SubMatrix(int row, int col, int n) const {
            SquareMatrix ret(n);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    ret(i, j) = this->operator()(row + i, col + j);
                }
            }
            return ret;
        }

        int GetN() const { return m_n; }

        SquareMatrix operator+(SquareMatrix const& rhs) {
            SquareMatrix ret(m_n);
            for (int i = 0; i < m_n; ++i) {
                for (int j = 0; j < m_n; ++j) {
                    ret(i, j) = this->operator()(i, j) + rhs(i, j);
                }
            }
            return ret;
        }
        SquareMatrix& operator+=(SquareMatrix const& rhs) {
            for (int i = 0; i < m_n; ++i) {
                for (int j = 0; j < m_n; ++j) {
                    this->operator()(i, j) += rhs(i, j);
                }
            }
            return *this;
        }
        SquareMatrix operator-(SquareMatrix const& rhs) {
            SquareMatrix ret(m_n);
            for (int i = 0; i < m_n; ++i) {
                for (int j = 0; j < m_n; ++j) {
                    ret(i, j) = this->operator()(i, j) - rhs(i, j);
                }
            }
            return ret;
        }
        SquareMatrix& operator-=(SquareMatrix const& rhs) {
            for (int i = 0; i < m_n; ++i) {
                for (int j = 0; j < m_n; ++j) {
                    this->operator()(i, j) -= rhs(i, j);
                }
            }
            return *this;
        }

        T& operator()(int row, int col) { return m_data[row * m_n + col]; }
        const T& operator()(int row, int col) const { return m_data[row * m_n + col]; }

        T* begin() { return m_data.begin(); }
        T* end() { return m_data.end(); }
        const T* begin() const { return m_data.begin(); }
        const T* end() const { return m_data.end(); }
    };
    
    template <typename T>
    void MatrixMultiply(const SquareMatrix<T>& a, const SquareMatrix<T>& b, SquareMatrix<T>& c) {
        if (a.GetN() != b.GetN()) {
            return;
        }

        for (int i = 0; i < a.GetN(); ++i) {
            for (int j = 0; j < b.GetN(); ++j) {
                c(i, j) = 0;
                for (int k = 0; k < a.GetN(); ++k) {
                    c(i, j) += a(i, k) * b(k, j);
                }
            }
        }
    }

    template <typename T>
    void MatrixMultiplyRecursive(const SquareMatrix<T>& a, const SquareMatrix<T>& b, SquareMatrix<T>& c) {
        if (a.GetN() != b.GetN() || a.GetN() & (a.GetN() - 1) != 0) {
            return;
        }
        MatrixMultiplyRecursiveAux(a, b, c, a.GetN(), 0, 0, 0, 0, 0, 0);
    }

    template <typename T>
    void MatrixMultiplyRecursiveAux(const SquareMatrix<T>& a, const SquareMatrix<T>& b, SquareMatrix<T>& c, int n, int a_row, int a_col, int b_row, int b_col, int c_row, int c_col) {
        if (n == 1) {
            c(c_row, c_col) += a(a_row, a_col) * b(b_row, b_col);
        }
        else {
            int half = n / 2;
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row, a_col, b_row, b_col, c_row, c_col);
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row, a_col, b_row, b_col + half, c_row, c_col + half);
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row + half, a_col, b_row, b_col, c_row + half, c_col);
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row + half, a_col, b_row, b_col + half, c_row + half, c_col + half);
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row, a_col + half, b_row + half, b_col, c_row, c_col);
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row, a_col + half, b_row + half, b_col + half, c_row, c_col + half);
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row + half, a_col + half, b_row + half, b_col, c_row + half, c_col);
            MatrixMultiplyRecursiveAux(a, b, c, half, a_row + half, a_col + half, b_row + half, b_col + half, c_row + half, c_col + half);
        }
    }

    template <typename T>
    void Strassen(const SquareMatrix<T>& a, const SquareMatrix<T>& b, SquareMatrix<T>& c) {
        if (a.GetN() != b.GetN() || a.GetN() & (a.GetN() - 1) != 0) {
            return;
        }
        StrassenAux(a, b, c, a.GetN(), 0, 0, 0, 0, 0, 0);
    }

    template <typename T>
    void StrassenAux(const SquareMatrix<T>& a, const SquareMatrix<T>& b, SquareMatrix<T>& c, int n, int a_row, int a_col, int b_row, int b_col, int c_row, int c_col) {
        if (n == 1) {
            c(c_row, c_col) += a(a_row, a_col) * b(b_row, b_col);
        }
        else {
            int half = n / 2;
            SquareMatrix<T> s1(half), s2(half), s3(half), s4(half), s5(half), s6(half), s7(half), s8(half), s9(half), s10(half);
            SquareMatrix<T> p1(half), p2(half), p3(half), p4(half), p5(half), p6(half), p7(half);

            s1 = b.SubMatrix(b_row, b_col + half, half) - b.SubMatrix(b_row + half, b_col + half, half);
            s2 = a.SubMatrix(a_row, a_col, half) + a.SubMatrix(a_row, a_col + half, half);
            s3 = a.SubMatrix(a_row + half, a_col, half) - a.SubMatrix(a_row, a_col, half);
            s4 = b.SubMatrix(b_row + half, b_col, half) - b.SubMatrix(b_row, b_col, half);
            s5 = a.SubMatrix(a_row, a_col, half) + a.SubMatrix(a_row + half, a_col + half, half);
            s6 = b.SubMatrix(b_row, b_col, half) + b.SubMatrix(b_row + half, b_col + half, half);
            s7 = a.SubMatrix(a_row, a_col + half, half) - a.SubMatrix(a_row + half, a_col + half, half);
            s8 = b.SubMatrix(b_row + half, b_col, half) + b.SubMatrix(b_row + half, b_col + half, half);
            s9 = a.SubMatrix(a_row, a_col, half) - a.SubMatrix(a_row + half, a_col, half);
            s10 = b.SubMatrix(b_row, b_col, half) + b.SubMatrix(b_row, b_col + half, half);

            StrassenAux(a, s1, p1, half, a_row, a_col, 0, 0, 0, 0);
            StrassenAux(s2, b, p2, half, 0, 0, b_row + half, b_col + half, 0, 0);
            StrassenAux(s3, b, p3, half, 0, 0, b_row, b_col, 0, 0);
            StrassenAux(a, s4, p4, half, a_row + half, a_col + half, 0, 0, 0, 0);
            StrassenAux(s5, s6, p5, half, 0, 0, 0, 0, 0, 0);
            StrassenAux(s7, s8, p6, half, 0, 0, 0, 0, 0, 0);
            StrassenAux(s9, s10, p7, half, 0, 0, 0, 0, 0, 0);

            SquareMatrix<T> c11 = p5 + p4 - p2 + p6;
            SquareMatrix<T> c12 = p1 + p2;
            SquareMatrix<T> c21 = p3 + p4;
            SquareMatrix<T> c22 = p5 + p1 - p3 - p7;

            for (int i = 0; i < half; ++i) {
                for (int j = 0; j < half; ++j) {
                    c(c_row + i, c_col + j) += c11(i, j);
                    c(c_row + i, c_col + j + half) += c12(i, j);
                    c(c_row + i + half, c_col + j) += c21(i, j);
                    c(c_row + i + half, c_col + j + half) += c22(i, j);
                }
            }
        }
    }
}

#endif // _ALGORITHM_MATRIX_MULTIPLY_HPP_
