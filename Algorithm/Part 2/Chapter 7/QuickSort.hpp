#ifndef _ALGORITHM_QUICKSORT_HPP_
#define _ALGORITHM_QUICKSORT_HPP_

#include <stl_algorithm.hpp>

namespace Algorithm {

    template <typename Iterator, typename Compare>
    Iterator partition(Iterator first, Iterator last, Compare comp) {
        if (first == last)
            return first;

        auto i = first;
        for (auto j = first; j != last; ++j) {
            if (comp(*j, *first)) {
                ++i;
                TinySTL::iter_swap(i, j);
            }
        }
        TinySTL::iter_swap(i, first);
        return i;
    }

    template <typename Iterator, typename Compare>
    void quick_sort(Iterator first, Iterator last, Compare comp) {
        if (first == last)
            return;

        auto middle = Algorithm::partition(first, last, comp);
        quick_sort(first, middle, comp);
        quick_sort(std::next(middle), last, comp);
    }


} // namespace Algorithm

#endif // !_ALGORITHM_INSERTIONSORT_HPP_
