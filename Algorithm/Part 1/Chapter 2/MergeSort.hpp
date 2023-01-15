#ifndef _ALGORITHM_MERGESORT_HPP_
#define _ALGORITHM_MERGESORT_HPP_

#include <stl_vector.hpp>

namespace Algorithm {

    template <typename Iterator, typename Compare>
    void Merge(Iterator first, Iterator middle, Iterator last, Compare comp) {
        using ValueType = typename TinySTL::iterator_traits<Iterator>::value_type;
        TinySTL::vector<ValueType> left(first, middle);
        TinySTL::vector<ValueType> right(middle, last);
        auto left_iter = left.begin();
        auto right_iter = right.begin();
        auto iter = first;
        while (left_iter != left.end() && right_iter != right.end()) {
            if (comp(*left_iter, *right_iter)) {
                *iter = *left_iter;
                ++left_iter;
            } else {
                *iter = *right_iter;
                ++right_iter;
            }
            ++iter;
        }
        while (left_iter != left.end()) {
            *iter = *left_iter;
            ++left_iter;
            ++iter;
        }
        while (right_iter != right.end()) {
            *iter = *right_iter;
            ++right_iter;
            ++iter;
        }
    }

    template <typename Iterator, typename Compare>
    void MergeSort(Iterator first, Iterator last, Compare comp) {
        if (first == last || std::next(first) == last) {
            return;
        }
        auto middle = std::next(first, std::distance(first, last) / 2);
        MergeSort(first, middle, comp);
        MergeSort(middle, last, comp);
        Merge(first, middle, last, comp);
    }

}

#endif // !_ALGORITHM_MERGESORT_HPP_
