#ifndef _ALGORITHM_MERGESORT_HPP_
#define _ALGORITHM_MERGESORT_HPP_

#include <stl_algorithm.hpp>
#include <stl_vector.hpp>

namespace Algorithm {

    /// @brief Merge elements locally with buffer.
    template <typename Iterator, typename Compare>
    void Merge(Iterator first, Iterator middle, Iterator last, Compare comp) {
        using ValueType = typename TinySTL::iterator_traits<Iterator>::value_type;
        // Copy the data to buffer.
        TinySTL::vector<ValueType> left(first, middle);
        TinySTL::vector<ValueType> right(middle, last);
        TinySTL::merge(left.begin(), left.end(), right.begin(), right.end(), first, comp);
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
