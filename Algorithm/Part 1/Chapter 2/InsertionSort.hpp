#ifndef _ALGORITHM_INSERTIONSORT_HPP_
#define _ALGORITHM_INSERTIONSORT_HPP_

namespace Algorithm {

    template <typename Iterator, typename Compare>
    void InsertionSort(Iterator first, Iterator last, Compare comp) {
        if (first == last)
            return;
        for (Iterator i = first + 1; i != last; ++i) {
            auto key = *i;
            Iterator j = i - 1;
            for (; j >= first && comp(key, *j); --j)
                *(j + 1) = *j;
            *(j + 1) = key;
        }
    }

} // namespace Algorithm

#endif // !_ALGORITHM_INSERTIONSORT_HPP_
