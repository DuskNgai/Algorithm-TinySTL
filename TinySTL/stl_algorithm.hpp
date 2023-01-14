#ifndef _TINYSTL_ALGORITHM_HPP_
#define _TINYSTL_ALGORITHM_HPP_

#include <cstdlib>
#include <stl_heap.hpp>
#include <stl_iterator.hpp>
#include <stl_pair.hpp>
#include <stl_type_traits.hpp>

namespace TinySTL {

    //! ----- Comparison ------ !//

    //! O(1)
    template <typename T>
    constexpr const T& min(const T& a, const T& b) {
        return b < a ? b : a;
    }

    //! O(1)
    template <typename T, typename Compare>
    constexpr const T& min(const T& a, const T& b, Compare comp) {
        return comp(b, a) ? b : a;
    }

    //! O(1)
    template <typename T>
    constexpr const T& max(const T& a, const T& b) {
        return a < b ? b : a;
    }

    //! O(1)
    template <typename T, typename Compare>
    constexpr const T& max(const T& a, const T& b, Compare comp) {
        return comp(a, b) ? b : a;
    }

    //! O(1)
    template <typename T>
    constexpr const T& median(const T& a, const T& b, const T& c) {
        if (a < b) {
            if (b < c)
                return b;
            else if (a < c)
                return c;
            else
                return a;
        }
        else if (a < c)
            return a;
        else if (b < c)
            return c;
        else
            return b;
    }

    //! O(1)
    template <typename T, typename Compare>
    constexpr const T& median(const T& a, const T& b, const T& c, Compare comp) {
        if (comp(a, b)) {
            if (comp(b, c))
                return b;
            else if (comp(a, c))
                return c;
            else
                return a;
        }
        else if (comp(a, c))
            return a;
        else if (comp(b, c))
            return c;
        else
            return b;
    }

    //! ----- Memory Operations ------ !//

    //! O(n)
    // Fill the same `value` in [`first`, `last`).
    template <typename ForwardIterator, typename T>
    void fill(ForwardIterator first, ForwardIterator last, const T& value) {
        for (; first != last; ++first) {
            *first = value;
        }
    }

    //! O(n)
    // Fill `n` same `value` start from `first`.
    template <typename OutputIterator, typename Size, typename T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
        for (; n > 0; --n, ++first) {
            *first = value;
        }
        return first;
    }

    //! O(n)
    // InputIterator version.
    template <typename InputIterator, typename OutputIterator>
    OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
        for (; first != last; ++first, ++result) {
            *result = *first;
        }
        return result;
    }

    //! O(n)
    // RandomAccessIterator version
    template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
    inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
        for (Distance n = last - first; n > 0; --n, ++result, ++first) {
            *result = *first;
        }
        return result;
    }

    // RandomAccessIterator version
    template <typename RandomAccessIterator, typename OutputIterator>
    inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
        return __copy_d(first, last, result, difference_type(first));
    }

    // Dispatch based on the iterator category.
    template <typename InputIterator, typename OutputIterator>
    struct __copy_dispatch {
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
            return __copy(first, last, result, iterator_category(first));
        }
    };

    //! O(1)
    // If has_trivial_assignment_operator.
    template <typename T>
    inline T* __copy_trivial(const T* first, const T* last, T* result, __true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }

    // If no_trivial_assignment_operator.
    template <typename T>
    inline T* __copy_trivial(const T* first, const T* last, T* result, __false_type) {
        // Pointer is a kind of RandomAccessIterator.
        return __copy_d(first, last, result, (ptrdiff_t*)(0));
    }

    template <typename T>
    struct __copy_dispatch<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            using assign_op = typename __type_traits<T>::has_trivial_assignment_operator;
            return __copy_trivial(first, last, result, assign_op());
        }
    };

    template <typename T>
    struct __copy_dispatch<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            using assign_op = typename __type_traits<T>::has_trivial_assignment_operator;
            return __copy_trivial(first, last, result, assign_op());
        }
    };

    //! O(n)
    // Copy the value in [`first`, `last`) to container start from `output`.
    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator output) {
        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, output);
    }

    inline char* copy(const char* first, const char* last, char* result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    //! O(n)
    template <typename BidirectionalIterator1, typename BidirectionalIterator2, typename Distance>
    inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, bidirectional_iterator_tag, Distance*) {
        while (first != last) {
            *--result = *--last;
        }
        return result;
    }

    //! O(n)
    template <typename RandomAccessIterator, typename BidirectionalIterator, typename Distance>
    inline BidirectionalIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator result, random_access_iterator_tag, Distance*) {
        for (Distance n = last - first; n > 0; --n) {
            *--result = *--last;
        }
        return result;
    }

    template <typename InputIterator, typename OutputIterator>
    struct __copy_backward_dispatch {
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
            return __copy_backward(first, last, result, iterator_category(first), difference_type(first));
        }
    };

    //! O(1)
    template <typename T>
    T* __copy_backward_trivial(const T* first, const T* last, T* result, __true_type) {
        const ptrdiff_t len = last - first;
        memmove(result - len, first, sizeof(T) * len);
        return result - len;
    }

    template <typename T>
    T* __copy_backward_trivial(const T* first, const T* last, T* result, __false_type) {
        return __copy_backward(first, last, result, iterator_category(first), difference_type(first));
    }

    template <typename T>
    struct __copy_backward_dispatch<T*, T*> {
        T* operator()(T* first, T* last, T* result) {
            using assign_op = typename __type_traits<T>::has_trivial_assignment_operator;
            return __copy_backward_trivial(first, last, result, assign_op());
        }
    };

    template <typename T>
    struct __copy_backward_dispatch<const T*, T*> {
        T* operator()(const T* first, const T* last, T* result) {
            using assign_op = typename __type_traits<T>::has_trivial_assignment_operator;
            return __copy_backward_trivial(first, last, result, assign_op());
        }
    };

    //! O(n)
    // Copy the value in [`first`, `last`) to container end with `output` (exclusive).
    template <typename BidirectionalIterator1, typename BidirectionalIterator2>
    BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 output) {
        return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, output);
    }

    //! O(1)
    template <typename T>
    constexpr void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }

    //! O(1)
    // Swap two elements pointed by iterators.
    template <typename ForwardIterator1, typename ForwardIterator2>
    inline void iter_swap(ForwardIterator1 first, ForwardIterator2 second) {
        auto temp = *first;
        *first = *second;
        *second = temp;
    }

    //! O(n)
    // Similar to rotate but same length. Iterators can come from different containers.
    template <typename ForwardIterator1, typename ForwardIterator2>
    ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
        for (; first1 != last1; ++first1, ++first2) {
            TinySTL::iter_swap(first1, first2);
        }
        return first2;
    }

    //! O(n)
    // BidirectionalIterator version.
    template <typename BidirectionalIterator>
    void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
        while (true) {
            // Move last forward.
            if (first == last || first == --last) {
                return;
            }
            // Move first backward.
            else {
                TinySTL::iter_swap(++first, last);
            }
        }
    }

    //! O(n)
    // RandomAccessIterator version
    template <typename RandomAccessIterator>
    void __reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
        while (first < last) {
            TinySTL::iter_swap(first++, --last);
        }
    }

    //! O(n)
    // Reverse elements in container.
    template <typename BidirectionalIterator>
    inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
        __reverse(first, last, iterator_category(first));
    }

    //! O(n)
    // Reverse elements in container to another container.
    template <typename BidirectionalIterator, typename OutputIterator>
    OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
        while (first != last) {
            --last;
            *result = *last;
            ++result;
        }
        return result;
    }

    //! ----- Comparison ------ !//

    //! O(n)
    // Check between [`first1`, `last1`) and an iterator start from `first2`.
    template <typename InputIterator1, typename InputIterator2>
    constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        for (; first1 != last1; ++first1, ++first2) {
            if (*first1 != *first2) {
                return false;
            }
        }
        return true;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename BinaryOperator>
    constexpr bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryOperator op) {
        for (; first1 != last1; ++first1, ++first2) {
            if (!op(*first1, *first2)) {
                return false;
            }
        }
        return true;
    }

    //! O(n)
    // Return 0, 1.
    template <typename InputIterator1, typename InputIterator2>
    constexpr bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (*first1 < *first2)
                return true;
            if (*first2 < *first1)
                return false;
        }
        return first1 == last1 && first2 != last2;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename Compare>
    constexpr bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (comp(*first1, *first2))
                return true;
            if (comp(*first2, *first1))
                return false;
        }
        return first1 == last1 && first2 != last2;
    }

    //! O(n)
    bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) {
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, TinySTL::min(len1, len2));
        return (result != 0) ? (result < 0) : (len1 < len2);
    }

    //! O(n)
    bool lexicographical_compare(const char* first_1, const char* last_1, const char* first_2, const char* last_2) {
#if CHAR_MAX == SCHAR_MAX
        return lexicographical_compare((const signed char*)first_1, (const signed char*)last_1, (const signed char*)first_2, (const signed char*)last_2);
#else
        return lexicographical_compare((const unsigned char*)first_1, (const unsigned char*)last_1, (const unsigned char*)first_2, (const unsigned char*)last_2);
#endif
    }

    //! O(n)
    // Return -1, 0, 1.
    template <typename InputIterator1, typename InputIterator2>
    constexpr int lexicographical_compare_3way(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                return -1;
            }
            if (*first2 < *first1) {
                return 1;
            }
            ++first1;
            ++first2;
        }
        if (first2 == last2) {
            return !(first1 == last1);
        }
        else {
            return 1;
        }
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename Compare>
    constexpr int lexicographical_compare_3way(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                return -1;
            }
            if (comp(*first2, *first1)) {
                return 1;
            }
            ++first1;
            ++first2;
        }
        if (first2 == last2) {
            return !(first1 == last1);
        }
        else {
            return 1;
        }
    }

    //! O(n)
    int lexicographical_compare_3way(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) {
        const ptrdiff_t len1 = last1 - first1;
        const ptrdiff_t len2 = last2 - first2;
        const int result = memcmp(first1, first2, TinySTL::min(len1, len2));
        return (result != 0) ? result : ((len1 == len2) ? 0 : (len1 < len2 ? -1 : 1));
    }

    //! O(n)
    int lexicographical_compare_3way(const char* first_1, const char* last_1, const char* first_2, const char* last_2) {
#if CHAR_MAX == SCHAR_MAX
        return lexicographical_compare_3way((const signed char*)first_1, (const signed char*)last_1, (const signed char*)first_2, (const signed char*)last_2);
#else
        return lexicographical_compare_3way((const unsigned char*)first_1, (const unsigned char*)last_1, (const unsigned char*)first_2, (const unsigned char*)last_2);
#endif
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2>
    pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        while (first1 != last1 && *first1 == *first2) {
            ++first1;
            ++first2;
        }
        return pair<InputIterator1, InputIterator2>(first1, first2);
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename BinaryOperator>
    pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryOperator op) {
        while (first1 != last1 && op(*first1, *first2)) {
            ++first1;
            ++first2;
        }
        return pair<InputIterator1, InputIterator2>(first1, first2);
    }

    //! ----- Set Operations ------ !//

    //! O(n)
    // $S_1 \\supset S_2$
    // Suppose they are in the same (ascending or descending) order.
    template <typename InputIterator1, typename InputIterator2>
    bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                ++first1;
            }
            else if (*first2 < *first1) {
                return false;
            }
            else {
                ++first1;
                ++first2;
            }
        }
        return first2 == last2;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename Compare>
    bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                ++first1;
            }
            else if (comp(*first2, *first1)) {
                return false;
            }
            else {
                ++first1;
                ++first2;
            }
        }
        return first2 == last2;
    }

    //! O(n)
    // $S_1 \\cup S_2$
    // Suppose they are in the same (ascending or descending) order.
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                *result = *first1;
                ++first1;
            }
            else if (*first2 > *first1) {
                *result = *first2;
                ++first2;
            }
            else {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        // Append tails.
        result = TinySTL::copy(first1, last1, result);
        result = TinySTL::copy(first2, last2, result);
        return result;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                *result = *first1;
                ++first1;
            }
            else if (comp(*first2, *first1)) {
                *result = *first2;
                ++first2;
            }
            else {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        result = TinySTL::copy(first1, last1, result);
        result = TinySTL::copy(first2, last2, result);
        return result;
    }

    //! O(n)
    // $S_1 \\cap S_2$
    // Suppose they are in the same (ascending or descending) order.
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                ++first1;
            }
            else if (*first2 < *first1) {
                ++first2;
            }
            else {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                ++first1;
            }
            else if (comp(*first2, *first1)) {
                ++first2;
            }
            else {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    //! O(n)
    // $S_1 - S_2$
    // Suppose they are in the same (ascending or descending) order.
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (*first2 < *first1) {
                ++first2;
            }
            else {
                ++first1;
                ++first2;
            }
        }
        result = TinySTL::copy(first1, last1, result);
        return result;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (comp(*first2, *first1)) {
                ++first2;
            }
            else {
                ++first1;
                ++first2;
            }
        }
        result = TinySTL::copy(first1, last1, result);
        return result;
    }

    //! O(n)
    // $(S_1 - S_2) \\cup (S_2 - S_1)$
    // Suppose they are in the same (ascending or descending) order.
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 < *first2) {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (*first2 < *first1) {
                *result = *first2;
                ++first2;
                ++result;
            }
            else {
                ++first1;
                ++first2;
            }
        }
        result = TinySTL::copy(first1, last1, result);
        result = TinySTL::copy(first2, last2, result);
        return result;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first1, *first2)) {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (comp(*first2, *first1)) {
                *result = *first2;
                ++first2;
                ++result;
            }
            else {
                ++first1;
                ++first2;
            }
        }
        result = TinySTL::copy(first1, last1, result);
        result = TinySTL::copy(first2, last2, result);
        return result;
    }

    //! ----- Generation ------ !//

    //! O(n)
    // Apply a function to every element of a range.
    template <typename InputIterator, typename UnaryOperator>
    UnaryOperator for_each(InputIterator first, InputIterator last, UnaryOperator op) {
        for (; first != last; ++first) {
            op(*first);
        }
        return op;
    }

    //! O(n)
    // Generate values for a range, using `operator=`.
    template <typename ForwardIterator, typename Generator>
    void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
        for (; first != last; ++first) {
            *first = gen();
        }
    }

    //! O(n)
    // Generate values for n elements, using `operator=`.
    template <typename OutputIterator, typename Size, typename Generator>
    OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
        for (; n > 0; --n, ++first) {
            *first = gen();
        }
        return first;
    }

    //! O(n)
    // Transform one range into another range using unary operation.
    template <typename InputIterator, typename OutputIterator, typename UnaryOperator>
    OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperator op) {
        for (; first != last; ++first, ++result) {
            *result = op(*first);
        }
        return result;
    }

    //! O(n)
    // Transform two ranges into the other range using binary operation
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperator>
    OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryOperator op) {
        for (; first1 != last1; ++first1, ++first2, ++result) {
            *result = op(*first1, *first2);
        }
        return result;
    }

    //! O(n)
    // Random shuffle a range.
    template <typename RandomAccessIterator>
    inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        if (first == last) {
            return;
        }
        for (RandomAccessIterator i = first + 1; i != last; ++i) {
            TinySTL::iter_swap(i, first + rand() % ((i - first) + 1));
        }
    }

    //! O(n)
    template <typename RandomAccessIterator, typename RandomNumberGenerator>
    inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator& rng) {
        if (first == last) {
            return;
        }
        for (RandomAccessIterator i = first + 1; i != last; ++i) {
            TinySTL::iter_swap(i, first + rng((i - first) + 1));
        }
    }

    //! ----- Search ------ !//

    //! O(n)
    // Find a iterator point to the maximal element for a range.
    template <typename ForwardIterator>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
        if (first == last) {
            return last;
        }
        ForwardIterator result = first;
        while (++first != last) {
            if (*result < *first) {
                result = first;
            }
        }
        return result;
    }

    //! O(n)
    template <typename ForwardIterator, typename Compare>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
        if (first == last) {
            return last;
        }
        ForwardIterator result = first;
        while (++first != last) {
            if (comp(*result, *first)) {
                result = first;
            }
        }
        return result;
    }

    //! O(n)
    // Find a iterator point to the minimal element for a range.
    template <typename ForwardIterator>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
        if (first == last) {
            return last;
        }
        ForwardIterator result = first;
        while (++first != last) {
            if (*first < *result) {
                result = first;
            }
        }
        return result;
    }

    //! O(n)
    template <typename ForwardIterator, typename Compare>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
        if (first == last) {
            return last;
        }
        ForwardIterator result = first;
        while (++first != last) {
            if (comp(*first, *result)) {
                result = first;
            }
        }
        return result;
    }

    //! O(n)
    // Find the first element that is equal to `value`.
    // For RandomAccessIterator, vectorized version can be implemented.
    template <typename InputIterator, typename T>
    inline InputIterator find(InputIterator first, InputIterator last, const T& value) {
        while (first != last && *first != value) {
            ++first;
        }
        return first;
    }

    //! O(n)
    // Find the first element that satisfy the Predicate.
    // For RandomAccessIterator, vectorized version can be implemented.
    template <typename InputIterator, typename Predicate>
    inline InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
        while (first != last && !pred(*first)) {
            ++first;
        }
        return first;
    }

    //! O(n)
    // Find the first two equal adjacent elements.
    template <typename ForwardIterator>
    ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
        if (first == last) {
            return last;
        }
        ForwardIterator next = first;
        while (++next != last) {
            if (*first == *next) {
                return first;
            }
            first = next;
        }
        return last;
    }

    //! O(n)
    // Find the first two elements that satisfy the Predicate.
    template <typename ForwardIterator, typename BinaryOperator>
    ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryOperator op) {
        if (first == last) {
            return last;
        }
        ForwardIterator next = first;
        while (++next != last) {
            if (op(*first, *next)) {
                return first;
            }
            first = next;
        }
        return last;
    }

    //! O(n)
    // Count the number of elements in [`first, `last`) that are equal to `value`.
    template <typename InputIterator, typename T>
    typename iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T& value) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        for (; first != last; ++first) {
            if (*first == value) {
                ++n;
            }
        }
        return n;
    }

    //! O(n)
    // Count the number of elements in [`first`, `last`) that satisfy the Predicate.
    template <typename InputIterator, typename Predicate>
    typename iterator_traits<InputIterator>::difference_type count_if(InputIterator first, InputIterator last, Predicate pred) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        for (; first != last; ++first) {
            if (pred(*first)) {
                ++n;
            }
        }
        return n;
    }

    //! O(mn)
    // Search for the first occurrence of the sequence defined by [`first2`, `last2`) in the sequence defined by [`first1`, `last1`).
    template <typename ForwardIterator1, typename ForwardIterator2>
    inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
        // Length = 0.
        if (first1 == last1 || first2 == last2) {
            return last1;
        }

        // Length = 1.
        ForwardIterator2 temp = first2;
        if (++temp == last2) {
            return TinySTL::find(first1, last1, *first2);
        }

        // Length > 1.
        while (first1 != last1) {
            // First find the beginning of the sequence.
            first1 = TinySTL::find(first1, last1, *first2);
            if (first1 == last1) {
                return last1;
            }

            ForwardIterator1 curr1 = first1;
            ++curr1;
            // Length == 1, but we need > 1.
            if (curr1 == last1) {
                return last1;
            }
            ForwardIterator2 curr2 = first2;
            ++curr2;
            while (*curr1 == *curr2) {
                // Find the sequence.
                if (++curr2 == last2) {
                    return first1;
                }
                // At the end of the container.
                if (++curr1 == last1) {
                    return last1;
                }
            }
            ++first1;
        }

        return first1;
    }

    //! O(mn)
    template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryOperator>
    inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryOperator op) {
        // Length = 0.
        if (first1 == last1 || first2 == last2) {
            return last1;
        }

        // Length = 1.
        ForwardIterator2 temp = first2;
        if (++temp == last2) {
            ForwardIterator1 first = first1, last = last1;
            while (first != last && !op(*first, *first2)) {
                ++first;
            }
            return first;
        }

        // Length > 1.
        while (first1 != last1) {
            // First find the beginning of the sequence.
            while (first1 != last1 && !op(*first1, *first2)) {
                ++first1;
            }
            if (first1 == last1) {
                return last1;
            }

            ForwardIterator1 curr1 = first1;
            ++curr1;
            // Length == 1, but we need > 1.
            if (curr1 == last1) {
                return last1;
            }
            ForwardIterator2 curr2 = first2;
            ++curr2;
            while (op(*curr1, *curr2)) {
                // Find the sequence.
                if (++curr2 == last2) {
                    return first1;
                }
                // At the end of the container.
                if (++curr1 == last1) {
                    return last1;
                }
            }
            ++first1;
        }

        return first1;
    }

    // ! O(n)
    // Find successive elements in range that are equal to `value`.
    template <typename ForwardIterator, typename Integer, typename T>
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value) {
        if (count <= 0) {
            return first;
        }
        else {
            // Match the first.
            first = TinySTL::find(first, last, value);
            while (first != last) {
                // Match the rest.
                Integer n = count - 1;
                ForwardIterator i = first;
                ++i;
                while (i != last && n != 0 && *i == value) {
                    ++i;
                    --n;
                }
                // Find.
                if (n == 0) {
                    return first;
                }
                // Not find.
                else {
                    first = TinySTL::find(i, last, value);
                }
            }
        }
        return last;
    }

    // ! O(n)
    template <typename ForwardIterator, typename Integer, typename T, typename BinaryOperator>
    ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value, BinaryOperator op) {
        if (count <= 0) {
            return first;
        }
        else {
            // Match the first.
            while (first != last && !op(*first, value)) {
                ++first;
            }
            while (first != last) {
                // Match the rest.
                Integer n = count - 1;
                ForwardIterator i = first;
                ++i;
                while (i != last && n != 0 && op(*i, value)) {
                    ++i;
                    --n;
                }
                // Find.
                if (n == 0) {
                    return first;
                }
                // Not find.
                else {
                    first = i;
                    while (first != last && !op(*first, value)) {
                        ++first;
                    }
                }
            }
        }
        return last;
    }

    //! O(mn)
    // Find the first element in [`first1`, `last1`) that is also in [`first2`, `last2`).
    template <typename InputIterator, typename ForwardIterator>
    InputIterator find_first_of(InputIterator first1, ForwardIterator last1, ForwardIterator first2, ForwardIterator last2) {
        for (; first1 != last1; ++first1) {
            for (ForwardIterator it = first2; it != last2; ++it) {
                if (*first1 == *it) {
                    return first1;
                }
            }
        }
        return last1;
    }

    //! O(mn)
    template <typename InputIterator, typename ForwardIterator, typename BinaryOperator>
    InputIterator find_first_of(InputIterator first1, ForwardIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryOperator op) {
        for (; first1 != last1; ++first1) {
            for (ForwardIterator it = first2; it != last2; ++it) {
                if (op(*first1, *it)) {
                    return first1;
                }
            }
        }
        return last1;
    }

    //! O(mn)
    // ForwardIterator version.
    template <typename ForwardIterator1, typename ForwardIterator2>
    ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator2 last1, ForwardIterator2 first2, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag) {
        if (first2 == last2) {
            return last1;
        }
        else {
            ForwardIterator1 result = last1;
            while (true) {
                // Use search to next match elements.
                ForwardIterator1 new_result = TinySTL::search(first1, last1, first2, last2);
                if (new_result == last1) {
                    // If not found next match, return what we have found.
                    return result;
                }
                else {
                    // Prepare for next match.
                    result = new_result;
                    first1 = new_result;
                    ++first1;
                }
            }
        }
    }

    //! O(mn)
    template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryOperator>
    ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator2 last1, ForwardIterator2 first2, BinaryOperator op, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag) {
        if (first2 == last2) {
            return last1;
        }
        else {
            ForwardIterator1 result = last1;
            while (true) {
                ForwardIterator1 new_result = TinySTL::search(first1, last1, first2, last2, op);
                if (new_result == last1) {
                    return result;
                }
                else {
                    result = new_result;
                    first1 = new_result;
                    ++first1;
                }
            }
        }
    }

    //! O(mn)
    // BidirectionalIterator version, maybe faster from end.
    template <typename BidirectionalIterator1, typename BidirectionalIterator2>
    BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag) {
        using reviter1 = reverse_iterator<BidirectionalIterator1>;
        using reviter2 = reverse_iterator<BidirectionalIterator2>;

        reviter1 rlast1(first1);
        reviter1 rfirst1(last1);
        reviter2 rlast2(first2);
        reviter2 rfirst2(last2);
        reviter1 rresult = TinySTL::search(rfirst1, rlast1, rfirst2, rlast2);
        if (rresult == rlast1) {
            return last1;
        }
        else {
            // Rewind the iterator.
            BidirectionalIterator1 result = rresult.base();
            TinySTL::advance(result, -TinySTL::distance(first2, last2));
            return result;
        }
    }

    //! O(mn)
    template <typename BidirectionalIterator1, typename BidirectionalIterator2, typename BinaryOperator>
    BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag, BinaryOperator op) {
        using reviter1 = reverse_iterator<BidirectionalIterator1>;
        using reviter2 = reverse_iterator<BidirectionalIterator2>;

        reviter1 rlast1(first1);
        reviter1 rfirst1(last1);
        reviter2 rlast2(first2);
        reviter2 rfirst2(last2);
        reviter1 rresult = TinySTL::search(rfirst1, rlast1, rfirst2, rlast2, op);
        if (rresult == rlast1) {
            return last1;
        }
        else {
            BidirectionalIterator1 result = rresult.base();
            TinySTL::advance(result, -TinySTL::distance(first2, last2));
            return result;
        }
    }

    // Find the last occurrence of [`first1`, `last1`) from the end of the [`first2`, `last2`).
    template <typename ForwardIterator1, typename ForwardIterator2>
    inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
        using category1 = typename iterator_traits<ForwardIterator1>::iterator_category;
        using category2 = typename iterator_traits<ForwardIterator2>::iterator_category;

        return __find_end(first1, last1, first2, last2, category1(), category2());
    }

    template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryOperator>
    inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryOperator op) {
        using category1 = typename iterator_traits<ForwardIterator1>::iterator_category;
        using category2 = typename iterator_traits<ForwardIterator2>::iterator_category;

        return __find_end(first1, last1, first2, last2, op, category1(), category2());
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Distance>
    ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*) {
        Distance len = TinySTL::distance(first, last);

        while (len > 0) {
            Distance half = len >> 1;
            ForwardIterator middle = first;
            TinySTL::advance(middle, half);
            if (*middle < value) {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else {
                len = half;
            }
        }
        return first;
    }

    //! O(logn)
    // A way of binary search, return the first position that no less than the value.
    template <typename ForwardIterator, typename T>
    inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
        return __lower_bound(first, last, value, difference_type(first));
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Distance, typename Compare>
    ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, Compare comp) {
        Distance len = distance(first, last);

        while (len > 0) {
            Distance half = len >> 1;
            ForwardIterator middle = first;
            TinySTL::advance(middle, half);
            if (comp(*middle, value)) {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else {
                len = half;
            }
        }
        return first;
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Compare>
    inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
        return __lower_bound(first, last, value, difference_type(first), comp);
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Distance>
    ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*) {
        Distance len = TinySTL::distance(first, last);

        while (len > 0) {
            Distance half = len >> 1;
            ForwardIterator middle = first;
            TinySTL::advance(middle, half);
            if (value < *middle) {
                len = half;
            }
            else {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }
        return first;
    }

    //! O(logn)
    // A way of binary search, return the first position that bigger than the value.
    template <typename ForwardIterator, typename T>
    inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
        return __upper_bound(first, last, value, difference_type(first));
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Distance, typename Compare>
    ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, Compare comp) {
        Distance len = TinySTL::distance(first, last);

        while (len > 0) {
            Distance half = len >> 1;
            ForwardIterator middle = first;
            TinySTL::advance(middle, half);
            if (comp(value, *middle)) {
                len = half;
            }
            else {
                first = middle;
                ++first;
                len = len - half - 1;
            }
        }
        return first;
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Compare>
    inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
        return __upper_bound(first, last, value, difference_type(first), comp);
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Distance>
    pair<ForwardIterator, ForwardIterator> __equal_range(ForwardIterator first, ForwardIterator last, const T& value, Distance*) {
        Distance len = TinySTL::distance(first, last);

        while (len > 0) {
            Distance half = len >> 1;
            ForwardIterator middle = first;
            TinySTL::advance(middle, half);
            if (*middle < value) {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else if (value < *middle) {
                len = half;
            }
            else {
                ForwardIterator left = TinySTL::lower_bound(first, middle, value);
                TinySTL::advance(first, len);
                ForwardIterator right = TinySTL::upper_bound(++middle, first, value);
                return pair<ForwardIterator, ForwardIterator>(left, right);
            }
        }
        return pair<ForwardIterator, ForwardIterator>(first, first);
    }

    //! O(logn)
    // Find the equal range, a.k.a, [lower bound, upper bound).
    template <typename ForwardIterator, typename T>
    inline pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value) {
        return __equal_range(first, last, value, difference_type(first));
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Distance, typename Compare>
    pair<ForwardIterator, ForwardIterator> __equal_range(ForwardIterator first, ForwardIterator last, const T& value, Distance*, Compare comp) {
        Distance len = TinySTL::distance(first, last);

        while (len > 0) {
            Distance half = len >> 1;
            ForwardIterator middle = first;
            TinySTL::advance(middle, half);
            if (comp(*middle, value)) {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else if (comp(value, *middle)) {
                len = half;
            }
            else {
                ForwardIterator left = TinySTL::lower_bound(first, middle, value, comp);
                TinySTL::advance(first, len);
                ForwardIterator right = TinySTL::upper_bound(++middle, first, value, comp);
                return pair<ForwardIterator, ForwardIterator>(left, right);
            }
        }
        return pair<ForwardIterator, ForwardIterator>(first, first);
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Compare>
    inline pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
        return __equal_range(first, last, value, difference_type(first), comp);
    }

    //! O(logn)
    // Binary search the first equal value.
    template <typename ForwardIterator, typename T>
    bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
        // It only ensures that the value is not less than the target.
        ForwardIterator target = TinySTL::lower_bound(first, last, value);
        return target != last && !(value < *target);
    }

    //! O(logn)
    template <typename ForwardIterator, typename T, typename Compare>
    bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
        ForwardIterator target = TinySTL::lower_bound(first, last, value, comp);
        return target != last && !comp(value, *target);
    }

    //! ----- Replace ------ !//

    //! O(n)
    // Replace all the elements that are equal to `old_value` to `new_value`.
    template <typename ForwardIterator, typename T>
    void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
        for (; first != last; ++first) {
            if (*first == old_value) {
                *first = new_value;
            }
        }
    }

    //! O(n)
    // Replace all the elements that satisfy the Predicate to `new_value`.
    template <typename ForwardIterator, typename Predicate, typename T>
    void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                *first = new_value;
            }
        }
    }

    //! O(n)
    // Similar to replace, the output container maybe different.
    template <typename InputIterator, typename OutputIterator, typename T>
    OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
        for (; first != last; ++first, ++result) {
            *result = (*first == old_value ? new_value : *first);
        }
        return result;
    }

    //! O(n)
    // Similar to replace_if, the output container maybe different.
    template <typename InputIterator, typename OutputIterator, typename Predicate, typename T>
    OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_value) {
        for (; first != last; ++first, ++result) {
            *result = (pred(*first) ? new_value : *first);
        }
        return result;
    }

    //! O(n)
    // Copy all elements that are not equal to `value` to `result`, they can be the same container.
    // UB when the container expands.
    template <typename InputIterator, typename OutputIterator, typename T>
    OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
        for (; first != last; ++first) {
            if (*first != value) {
                *result = *first;
                ++result;
            }
        }
        return result;
    }

    //! O(n)
    // Copy all elements that not satisfy the `Predicate` to `result`, they can be the same container.
    // UB when the container expands.
    template <typename InputIterator, typename OutputIterator, typename Predicate>
    OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred) {
        for (; first != last; ++first) {
            if (!pred(*first)) {
                *result = *first;
                ++result;
            }
        }
        return result;
    }

    //! O(n)
    // Remove all the elements that not equal to `value`.
    template <typename ForwardIterator, typename T>
    ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
        first = TinySTL::find(first, last, value);
        ForwardIterator next = first;
        return first == last ? first : TinySTL::remove_copy(++next, last, first, value);
    }

    //! O(n)
    // Remove all the elements that not satisfy the `Predicate`.
    template <typename ForwardIterator, typename Predicate>
    ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
        first = TinySTL::find_if(first, last, pred);
        ForwardIterator next = first;
        return first == last ? first : TinySTL::remove_copy_if(++next, last, first, pred);
    }

    //! O(n)
    // ForwardIterator version.
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator __unique_copy(InputIterator first, InputIterator last, ForwardIterator result, forward_iterator_tag) {
        *result = *first;
        while (++first != last) {
            if (*result != *first) {
                *++result = *first;
            }
        }
        return ++result;
    }

    //! O(n)
    // OutputIterator version.
    template <typename InputIterator, typename OutputIterator>
    OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, output_iterator_tag) {
        auto value = *first;
        *result = value;
        while (++first != last) {
            if (value != *first) {
                value = *first;
                *++result = value;
            }
        }
        return ++result;
    }

    //! O(n)
    // Remove duplicate nearby elements another container.
    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
        if (first == last) {
            return result;
        }
        return __unique_copy(first, last, result, iterator_category(result));
    }

    //! O(n)
    // ForwardIterator version.
    template <typename InputIterator, typename ForwardIterator, typename BinaryOperator>
    ForwardIterator __unique_copy(InputIterator first, InputIterator last, ForwardIterator result, forward_iterator_tag, BinaryOperator op) {
        *result = *first;
        while (++first != last) {
            if (!op(*result, *first)) {
                *++result = *first;
            }
        }
        return ++result;
    }

    //! O(n)
    // OutputIterator version.
    template <typename InputIterator, typename OutputIterator, typename BinaryOperator>
    OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, output_iterator_tag, BinaryOperator op) {
        auto value = *first;
        *result = value;
        while (++first != last) {
            if (!op(value, *first)) {
                value = *first;
                *++result = value;
            }
        }
        return ++result;
    }

    //! O(n)
    // Remove duplicate nearby elements another container.
    template <typename InputIterator, typename OutputIterator, typename BinaryOperator>
    inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result, BinaryOperator op) {
        if (first == last) {
            return result;
        }
        return __unique_copy(first, last, result, iterator_category(result), op);
    }

    //! O(n)
    // Remove duplicate nearby elements.
    template <typename ForwardIterator>
    ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
        first = TinySTL::adjacent_find(first, last);
        return TinySTL::unique_copy(first, last, first);
    }

    //! O(n)
    // Remove duplicate nearby elements.
    template <typename ForwardIterator, typename BinaryOperator>
    ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryOperator op) {
        first = TinySTL::adjacent_find(first, last, op);
        return TinySTL::unique_copy(first, last, first, op);
    }

    //! ----- Rotate ----- !//

    //! O(logn)
    template <typename EuclideanRingElement>
    EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
        while (n != 0) {
            EuclideanRingElement t = m % n;
            m = n;
            n = t;
        }
        return m;
    }

    //! O(n)
    // ForwardIterator version.
    template <typename ForwardIterator, typename Distance>
    ForwardIterator __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance*, forward_iterator_tag) {
        ForwardIterator first2 = middle;
        do {
            TinySTL::swap(*first++, *first2++);
            if (first == middle) {
                middle = first2;
            }
        } while (first2 != last);

        ForwardIterator new_middle = first;
        first2 = middle;
        while (first2 != last) {
            TinySTL::swap(*first++, *first2++);
            if (first == middle) {
                middle = first2;
            } else if (first2 == last) {
                first2 = middle;
            }
        }
        return new_middle;
    }

    //! O(n)
    // BidirectionalIterator version.
    template <typename BidirectionalIterator, typename Distance>
    BidirectionalIterator __rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance*, bidirectional_iterator_tag) {
        TinySTL::__reverse(first, middle, bidirectional_iterator_tag());
        TinySTL::__reverse(middle, last, bidirectional_iterator_tag());

        while (first != middle && middle != last) {
            TinySTL::swap(*first++, *--last);
        }

        if (first == middle) {
            TinySTL::__reverse(middle, last, bidirectional_iterator_tag());
            return last;
        } else {
            TinySTL::__reverse(first, middle, bidirectional_iterator_tag());
            return first;
        }
    }

    //! O(n)
    // RandomAccessIterator version.
    template <typename RandomAccessIterator, typename Distance>
    RandomAccessIterator __rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance*, random_access_iterator_tag) {
        Distance n = last - first;
        Distance k = middle - first;
        Distance l = n - k;
        RandomAccessIterator new_middle = first + l;

        if (k == l) {
            TinySTL::swap_ranges(first, middle, middle);
            return new_middle;
        }

        Distance d = TinySTL::__gcd(n, k);
        for (Distance i = 0; i < d; ++i) {
            auto tmp = *first;
            RandomAccessIterator p = first;
            if (k < l) {
                for (Distance j = 0; j < l / d; ++j) {
                    if (p > (first + l)) {
                        *p = *(p - l);
                        p -= l;
                    }
                    *p = *(p + k);
                    p += k;
                }
            } else {
                for (Distance j = 0; j < k / d - 1; ++j) {
                    if (p < (last - k)) {
                        *p = *(p + k);
                        p += k;
                    }
                    *p = *(p - l);
                    p -= l;
                }
            }
            *p = tmp;
            ++first;
        }
        return new_middle;
    }

    //! O(n)
    // Move [`middle`, `last`) to the front of [`first`, `middle`).
    template <typename ForwardIterator>
    inline ForwardIterator rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
        if (first == middle) {
            return last;
        }
        if (last == middle) {
            return first;
        }
        return __rotate(first, middle, last, difference_type(first), iterator_category(first));
    }

    //! O(n)
    // Similar to rotate, output to different container.
    template <typename ForwardIterator, typename OutputIterator>
    OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
        result = TinySTL::copy(middle, last, result);
        result = TinySTL::copy(first, middle, result);
        return result;
    }

    //! ----- Sorting ----- !//

    //! O(nlogm)
    template <typename RandomAccessIterator, typename T>
    void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*) {
        // Maintain a maximum heap from `first` to `middle`.
        TinySTL::make_heap(first, middle);
        for (RandomAccessIterator i = middle; i < last; ++i) {
            // For the rest elements, if bigger than top, then it is too large.
            if (*i < *first) {
                // Adjust the heap by inserting the new element into the heap.
                __pop_heap(first, middle, i, difference_type(first), T(*i));
            }
        }
        // Heap sort in ascending order.
        TinySTL::sort_heap(first, middle);
    }

    //! O(nlogm)
    template <typename RandomAccessIterator, typename T, typename Compare>
    void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*, Compare comp) {
        TinySTL::make_heap(first, middle, comp);
        for (RandomAccessIterator i = middle; i < last; ++i) {
            if (comp(*i, *first)) {
                __pop_heap(first, middle, i, difference_type(first), T(*i), comp);
            }
        }
        TinySTL::sort_heap(first, middle, comp);
    }

    //! O(nlogm)
    // Move the smallest elements from `first` to `last` to the range from `first` to `middle` using heap sort.
    template <typename RandomAccessIterator>
    inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
        __partial_sort(first, middle, last, value_type(first));
    }

    //! O(nlogm)
    template <typename RandomAccessIterator, typename Compare>
    inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp) {
        __partial_sort(first, middle, last, value_type(first), comp);
    }

    //! O(nlogm)
    template <typename InputIterator, typename RandomAccessIterator, typename Distance, typename T>
    RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Distance*, T*) {
        if (result_first == result_last) {
            return result_first;
        }
        // [`result_first`, `result_middle`) may be longer than [`first`, `last`).
        RandomAccessIterator result_middle = result_first;
        while (first != last && result_middle != result_last) {
            *result_middle = *first;
            ++result_middle;
            ++first;
        }
        TinySTL::make_heap(result_first, result_middle);
        while (first != last) {
            if (*first < *result_first) {
                __adjust_heap(result_first, Distance(0), Distance(result_middle - result_first), T(*first));
            }
            ++first;
        }
        TinySTL::sort_heap(result_first, result_middle);
        return result_middle;
    }

    //! O(nlogm)
    // Same as partial_sort, but output to [`result_first`, `result_last`).
    template <typename InputIterator, typename RandomAccessIterator>
    inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last) {
        return __partial_sort_copy(first, last, result_first, result_last, difference_type(result_first), value_type(first));
    }

    //! O(nlogm)
    template <typename InputIterator, typename RandomAccessIterator, typename Compare, typename Distance, typename T>
    RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Distance*, T*, Compare comp) {
        if (result_first == result_last) {
            return result_first;
        }
        RandomAccessIterator result_middle = result_first;
        while (first != last && result_middle != result_last) {
            *result_middle = *first;
            ++result_middle;
            ++first;
        }
        TinySTL::make_heap(result_first, result_middle, comp);
        while (first != last) {
            if (comp(*first, *result_first)) {
                __adjust_heap(result_first, Distance(0), Distance(result_middle - result_first), T(*first), comp);
            }
            ++first;
        }
        TinySTL::sort_heap(result_first, result_middle, comp);
        return result_middle;
    }

    //! O(nlogm)
    template <typename InputIterator, typename RandomAccessIterator, typename Compare>
    inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp) {
        return __partial_sort_copy(first, last, result_first, result_last, difference_type(result_first), value_type(first), comp);
    }

    //! O(n)
    // ForwardIterator version.
    template <typename ForwardIterator, typename Predicate>
    ForwardIterator __partition(ForwardIterator first, ForwardIterator last, Predicate pred, forward_iterator_tag) {
        if (first == last) {
            return first;
        }

        // Find the first element that does not satisfy the Predicate.
        while (pred(*first)) {
            if (++first == last) {
                return first;
            }
        }
        ForwardIterator next = first;

        // Swap the rest of the elements that satisfy the Predicate to the front.
        while (++next != last) {
            if (pred(*next)) {
                TinySTL::iter_swap(first, next);
                ++first;
            }
        }

        return first;
    }

    //! O(n)
    // BidirectionalIterator version.
    template <typename BidirectionalIterator, typename Predicate>
    BidirectionalIterator __partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred, bidirectional_iterator_tag) {
        while (true) {
            while (true) {
                if (first == last)
                    return first;
                else if (pred(*first))
                    ++first;
                else
                    break;
            }
            --last;
            while (true) {
                if (first == last)
                    return first;
                else if (!pred(*last))
                    --last;
                else
                    break;
            }
            TinySTL::iter_swap(first, last);
            ++first;
        }
    }

    //! O(n)
    // Elements that satisfies the `Predicate` will be move to front.
    template <typename ForwardIterator, typename Predicate>
    inline ForwardIterator partition(ForwardIterator first, ForwardIterator last, Predicate pred) {
        return __partition(first, last, pred, iterator_category(first));
    }

    //! O(n)
    template <typename ForwardIterator, typename Predicate>
    inline ForwardIterator __stable_partition(ForwardIterator first, ForwardIterator last, Predicate pred) {
        typename iterator_traits<ForwardIterator>::difference_type len = distance(first, last);

        // The key of the algorithm.
        // Once the a range of 2 elements is sorted, we can "sort" the whole range.
        if (len == 1) {
            return pred(*first) ? last : first;
        }

        // "Sort" by rorating the satisfying elements to the front.
        ForwardIterator middle = first;
        TinySTL::advance(middle, len >> 1);
        return rotate(
            __stable_partition(first, middle, pred),
            middle,
            __stable_partition(middle, last, pred)
        );

        return first;
    }

    //! O(n)
    // Elements that satisfies the `Predicate` will be move to front.
    // Two elements that are equal to each other will stay at front if it was front before.
    template <typename ForwardIterator, typename Predicate>
    inline ForwardIterator stable_partition(ForwardIterator first, ForwardIterator last, Predicate pred) {
        if (first == last) {
            return first;
        }
        else {
            return __stable_partition(first, last, pred);
        }
    }

    //! O(n)
    // Silimar to BidirectionalIterator version of `__partition`.
    template <typename RandomAccessIterator, typename T>
    RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot) {
        while (true) {
            while (*first < pivot) {
                ++first;
            }
            --last;
            while (pivot < *last) {
                --last;
            }
            if (!(first < last)) {
                return first;
            }
            TinySTL::iter_swap(first, last);
            ++first;
        }
    }

    //! O(n)
    template <typename RandomAccessIterator, typename T, typename Compare>
    RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot, Compare comp) {
        while (true) {
            while (comp(*first, pivot)) {
                ++first;
            }
            --last;
            while (comp(pivot, *last)) {
                --last;
            }
            if (!(first < last)) {
                return first;
            }
            TinySTL::iter_swap(first, last);
            ++first;
        }
    }

    //! O(n)
    // Insert a value to a sorted range from back to front.
    template <typename RandomAccessIterator, typename T>
    void __unguarded_linear_insert(RandomAccessIterator last, T value) {
        RandomAccessIterator next = last;
        --next;
        while (value < *next) {
            *last = *next;
            last = next;
            --next;
        }
        *last = value;
    }

    //! O(n)
    template <typename RandomAccessIterator, typename T, typename Compare>
    void __unguarded_linear_insert(RandomAccessIterator last, T value, Compare comp) {
        RandomAccessIterator next = last;
        --next;
        while (comp(value, *next)) {
            *last = *next;
            last = next;
            --next;
        }
        *last = value;
    }

    //! O(n)
    template <typename RandomAccessIterator, typename T>
    inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*) {
        T value = *last;
        // If the value is the smallest.
        if (value < *first) {
            TinySTL::copy_backward(first, last, last + 1);
            *first = value;
        }
        else {
            __unguarded_linear_insert(last, value);
        }
    }

    //! O(n)
    template <typename RandomAccessIterator, typename T, typename Compare>
    inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp) {
        T value = *last;
        if (comp(value, *first)) {
            TinySTL::copy_backward(first, last, last + 1);
            *first = value;
        }
        else {
            __unguarded_linear_insert(last, value, comp);
        }
    }

    //! O(n^2)
    template <typename RandomAccessIterator>
    void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (first == last)
            return;
        for (RandomAccessIterator i = first + 1; i != last; ++i) {
            __linear_insert(first, i, value_type(first));
        }
    }

    //! O(n^2)
    template <typename RandomAccessIterator, typename Compare>
    void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (first == last)
            return;
        for (RandomAccessIterator i = first + 1; i != last; ++i) {
            __linear_insert(first, i, value_type(first), comp);
        }
    }

    static constexpr int threshold = 16;

    //! O(threshold^2)
    template <typename RandomAccessIterator>
    void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (last - first > threshold) {
            // In first range, `copy_backward` used in `__insertion_sort` maybe faster.
            __insertion_sort(first, first + threshold);
            // In the rest part, we just need to move one by one in each range.
            for (RandomAccessIterator i = first + threshold; i != last; ++i) {
                __unguarded_linear_insert(i, *i);
            }
        }
        else {
            __insertion_sort(first, last);
        }
    }

    //! O(threshold^2)
    template <typename RandomAccessIterator, typename Compare>
    void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (last - first > threshold) {
            __insertion_sort(first, first + threshold, comp);
            for (RandomAccessIterator i = first + threshold; i != last; ++i) {
                __unguarded_linear_insert(i, *i, comp);
            }
        }
        else {
            __insertion_sort(first, last, comp);
        }
    }

    //! O(logn)
    template <typename Size>
    inline Size __log2(Size n) {
        Size k;
        for (k = 0; n != 1; n >>= 1) {
            ++k;
        }
        return k;
    }

    //! O(nlogn)
    template <typename RandomAccessIterator, typename T, typename Size>
    void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit) {
        while (last - first > threshold) {
            if (depth_limit == 0) {
                // Turns into heap sort if introsort works bad (to much partition).
                TinySTL::partial_sort(first, last, last);
                return;
            }
            --depth_limit;
            // Quick sort here, only do partition on the range whose size is longer than `threshold`.
            RandomAccessIterator pivot = __unguarded_partition(first, last, T(median(*first, *(first + ((last - first) / 2)), *(last - 1))));
            __introsort_loop(pivot, last, value_type(first), depth_limit);
            last = pivot;
        }
    }

    //! O(nlogn)
    template <typename RandomAccessIterator, typename T, typename Size, typename Compare>
    void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit, Compare comp) {
        while (last - first > threshold) {
            if (depth_limit == 0) {
                TinySTL::partial_sort(first, last, last, comp);
                return;
            }
            --depth_limit;
            RandomAccessIterator pivot = __unguarded_partition(first, last, T(median(*first, *(first + ((last - first) / 2)), *(last - 1), comp)), comp);
            __introsort_loop(pivot, last, value_type(first), depth_limit, comp);
            last = pivot;
        }
    }

    //! O(nlogn)
    template <typename RandomAccessIterator>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (first != last) {
            // Introsort will break a range into several ranges.
            // the size of each range is not longer than `threshold`.
            // Elements in previous range will not bigger than elements in posterior range.
            // So insertion sort can handle each range without affecting other ranges.
            __introsort_loop(first, last, value_type(first), __log2(last - first) * 2);
            // Sort the each small range.
            __final_insertion_sort(first, last);
        }
    }

    //! O(nlogn)
    template <typename RandomAccessIterator, typename Compare>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (first != last) {
            __introsort_loop(first, last, value_type(first), __log2(last - first) * 2, comp);
            __final_insertion_sort(first, last, comp);
        }
    }

    //! O(n)
    template <typename RandomAccessIterator, typename T>
    void __nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, T*) {
        while (last - first > 3) {
            RandomAccessIterator pivot = __unguarded_partition(first, last, T(TinySTL::median(*first, *(first + ((last - first) / 2)), *(last - 1))));
            if (pivot <= nth) {
                first = pivot;
            }
            else {
                last = pivot;
            }
        }
        __insertion_sort(first, last);
    }

    //! O(n)
    // Find the nth element of a unsorted range.
    template <typename RandomAccessIterator>
    inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last) {
        __nth_element(first, nth, last, value_type(first));
    }

    //! O(n)
    template <typename RandomAccessIterator, typename T, typename Compare>
    void __nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, T*, Compare comp) {
        while (last - first > 3) {
            RandomAccessIterator pivot = __unguarded_partition(first, last, T(TinySTL::median(*first, *(first + ((last - first) / 2)), *(last - 1), comp)), comp);
            if (pivot <= nth) {
                first = pivot;
            }
            else {
                last = pivot;
            }
        }
        __insertion_sort(first, last, comp);
    }

    //! O(n)
    template <typename RandomAccessIterator, typename Compare>
    inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare comp) {
        __nth_element(first, nth, last, value_type(first), comp);
    }

    //! O(n)
    // Merge two sorted ranges into one sorted range.
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                *result = *first2;
                ++first2;
            }
            else {
                *result = *first1;
                ++first1;
            }
            ++result;
        }
        result = TinySTL::copy(first1, last1, result);
        result = TinySTL::copy(first2, last2, result);
        return result;
    }

    //! O(n)
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1)) {
                *result = *first2;
                ++first2;
            }
            else {
                *result = *first1;
                ++first1;
            }
            ++result;
        }
        result = TinySTL::copy(first1, last1, result);
        result = TinySTL::copy(first2, last2, result);
        return result;
    }

    //! O(n)
    template <typename BidirectionalIterator, typename Distance>
    void __merge_without_buffer(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance len1, Distance len2) {
        if (len1 == 0 || len2 == 0) {
            return;
        }
        // Swap directly for length of 1 + 1.
        if (len1 + len2 == 2) {
            if (*middle < *first) {
                TinySTL::iter_swap(first, middle);
            }
            return;
        }
        BidirectionalIterator first_cut = first;
        BidirectionalIterator second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        // Binary search for the position of `new_middle` in the other half since they are sorted ranges.
        if (len1 > len2) {
            len11 = len1 / 2;
            TinySTL::advance(first_cut, len11);
            second_cut = TinySTL::lower_bound(middle, last, *first_cut);
            len22 = TinySTL::distance(middle, second_cut);
        }
        else {
            len22 = len2 / 2;
            TinySTL::advance(second_cut, len22);
            first_cut = TinySTL::upper_bound(first, middle, *second_cut);
            len11 = TinySTL::distance(first, first_cut);
        }
        // Rotate the smaller element to the front.
        BidirectionalIterator new_middle = TinySTL::rotate(first_cut, middle, second_cut);
        __merge_without_buffer(first, first_cut, new_middle, len11, len22);
        __merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
    }

    //! O(n)
    template <typename BidirectionalIterator, typename Distance, typename Compare>
    void __merge_without_buffer(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance len1, Distance len2, Compare comp) {
        if (len1 == 0 || len2 == 0) {
            return;
        }
        if (len1 + len2 == 2) {
            if (comp(*middle, *first)) {
                TinySTL::iter_swap(first, middle);
            }
            return;
        }
        BidirectionalIterator first_cut = first;
        BidirectionalIterator second_cut = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if (len1 > len2) {
            len11 = len1 / 2;
            TinySTL::advance(first_cut, len11);
            second_cut = TinySTL::lower_bound(middle, last, *first_cut, comp);
            len22 = TinySTL::distance(middle, second_cut);
        }
        else {
            len22 = len2 / 2;
            TinySTL::advance(second_cut, len22);
            first_cut = TinySTL::upper_bound(first, middle, *second_cut, comp);
            len11 = TinySTL::distance(first, first_cut);
        }
        BidirectionalIterator new_middle = TinySTL::rotate(first_cut, middle, second_cut);
        __merge_without_buffer(first, first_cut, new_middle, len11, len22, comp);
        __merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22, comp);
    }

    //! O(n)
    // Merge two sorted sub-ranges into one sorted range.
    template <typename BidirectionalIterator>
    void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
        if (first == middle || middle == last) {
            return;
        }
        __merge_without_buffer(first, middle, last, TinySTL::distance(first, middle), TinySTL::distance(middle, last));
    }

    //! O(n)
    template <typename BidirectionalIterator, typename Compare>
    void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Compare comp) {
        if (first == middle || middle == last) {
            return;
        }
        __merge_without_buffer(first, middle, last, TinySTL::distance(first, middle), TinySTL::distance(middle, last), comp);
    }

    //! Permutation !//

    //! O(n)
    // Permutation inplace.
    template <typename BidirectionalIterator>
    bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
        // 0
        if (first == last) {
            return false;
        }

        BidirectionalIterator i = first;
        ++i;
        // 1
        if (i == last) {
            return false;
        }
        i = last;
        --i;

        // From last to first.
        for (;;) {
            BidirectionalIterator j = i;
            --i;
            // If ascending.
            if (*i < *j) {
                BidirectionalIterator k = last;
                while (!(*i < *--k)) {
                }
                TinySTL::iter_swap(i, k);
                TinySTL::reverse(j, last);
                return true;
            }
            if (i == first) {
                TinySTL::reverse(first, last);
                return false;
            }
        }
    }

    //! O(n)
    template <typename BidirectionalIterator, typename Compare>
    bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
        if (first == last) {
            return false;
        }

        BidirectionalIterator i = first;
        ++i;
        if (i == last) {
            return false;
        }
        i = last;
        --i;

        for (;;) {
            BidirectionalIterator j = i;
            --i;
            if (comp(*i, *j)) {
                BidirectionalIterator k = last;
                while (!comp(*i, *--k)) {
                }
                TinySTL::iter_swap(i, k);
                TinySTL::reverse(j, last);
                return true;
            }
            if (i == first) {
                TinySTL::reverse(first, last);
                return false;
            }
        }
    }

    //! O(n)
    // Permutation inplace.
    template <typename BidirectionalIterator>
    bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
        if (first == last) {
            return false;
        }
        BidirectionalIterator i = first;
        ++i;
        if (i == last) {
            return false;
        }
        i = last;
        --i;

        for (;;) {
            BidirectionalIterator j = i;
            --i;
            if (*j < *i) {
                BidirectionalIterator k = last;
                while (!(*--k < *i)) {
                }
                TinySTL::iter_swap(i, k);
                TinySTL::reverse(j, last);
                return true;
            }
            if (i == first) {
                TinySTL::reverse(first, last);
                return false;
            }
        }
    }

    //! O(n)
    template <typename BidirectionalIterator, typename Compare>
    bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
        if (first == last) {
            return false;
        }
        BidirectionalIterator i = first;
        ++i;
        if (i == last) {
            return false;
        }
        i = last;
        --i;

        for (;;) {
            BidirectionalIterator j = i;
            --i;
            if (comp(*j, *i)) {
                BidirectionalIterator k = last;
                while (!comp(*--k, *i)) {
                }
                TinySTL::iter_swap(i, k);
                TinySTL::reverse(j, last);
                return true;
            }
            if (i == first) {
                TinySTL::reverse(first, last);
                return false;
            }
        }
    }

} // namespace TinySTL

#endif // !_TINYSTL_ALGORITHM_HPP_
