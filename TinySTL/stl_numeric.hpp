#ifndef _TINYSTL_NUMERIC_HPP_
#define _TINYSTL_NUMERIC_HPP_

#include <stl_function.hpp>

namespace TinySTL {

    template <typename InputIterator, typename T>
    constexpr T accumulate(InputIterator first, InputIterator last, T init) {
        for (; first != last; ++first) {
            init = init + *first;
        }
        return init;
    }

    template <typename InputIterator, typename T, typename BinaryOperator>
    constexpr T accumulate(InputIterator first, InputIterator last, T init, BinaryOperator op) {
        for (; first != last; ++first) {
            init = op(init, *first);
        }
        return init;
    }

    template <typename InputIterator, typename OutputIterator>
    constexpr OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
        if (first == last) {
            return result;
        }

        *result = *first;

        auto prev = *first;
        while (++first != last) {
            auto next = *first;
            // always next - prev
            *++result = next - prev;
            prev      = next;
        }
        return ++result;
    }

    template <typename InputIterator, typename OutputIterator, typename BinaryOperator>
    constexpr OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator op) {
        if (first == last) {
            return result;
        }

        *result = *first;

        auto prev = *first;
        while (++first != last) {
            auto next = *first;
            *++result = op(next, prev);
            prev      = next;
        }
        return ++result;
    }

    template <typename InputIterator, typename OutputIterator>
    constexpr OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
        if (first == last) {
            return result;
        }

        *result = *first;

        auto sum = *first;
        while (++first != last) {
            sum       = sum + *first;
            *++result = sum;
        }
        return ++result;
    }

    template <typename InputIterator, typename OutputIterator, typename BinaryOperator>
    constexpr OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator op) {
        if (first == last) {
            return result;
        }

        *result = *first;

        auto sum = *first;
        while (++first != last) {
            sum       = op(sum, *first);
            *++result = sum;
        }
        return ++result;
    }

    template <typename InputIterator1, typename InputIterator2, typename T>
    constexpr T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
        for (; first1 != last1; ++first1, ++first2) {
            init = init + (*first1 * *first2);
        }
        return init;
    }

    template <typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperator1, typename BinaryOperator2>
    constexpr T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperator1 op1, BinaryOperator2 op2) {
        for (; first1 != last1; ++first1, ++first2) {
            init = op1(init, op2(*first1, *first2));
        }
        return init;
    }

    template <typename T, typename Integer, typename MonoidOperator>
    constexpr T power(T x, Integer n, MonoidOperator op) {
        if (n == 0) {
            return identity_element(op);
        }
        else {
            // x ** n == x ** (2 ** a * b) == (x ** (2 ** a)) ** b
            while ((n & 1) == 0) {
                n >>= 1;
                x = op(x, x);
            }

            // Multiply and square for the rest of the bits.
            // (x ** (2 ** a)) ** b == result ** b
            T result = x;
            n >>= 1;
            while (n != 0) {
                x = op(x, x);
                if ((n & 1) != 0) {
                    result = op(result, x);
                }
                n >>= 1;
            }
            return result;
        }
    }

    template <typename T, typename Integer>
    constexpr T power(T x, Integer n) {
        return power(x, n, multiply<T>());
    }

    // `iota` is a greek alphabet.
    template <typename ForwardIterator, typename T>
    inline void iota(ForwardIterator first, ForwardIterator last, T value) {
        while (first != last) {
            *first++ = value++;
        }
    }

} // namespace TinySTL

#endif // !_TINYSTL_NUMERIC_HPP_
