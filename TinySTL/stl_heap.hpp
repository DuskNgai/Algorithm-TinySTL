#ifndef _TINYSTL_HEAP_HPP_
#define _TINYSTL_HEAP_HPP_

#include <stl_iterator.hpp>

namespace TinySTL {

    // Top index is 0 not 1.
    //      0
    //    /   \
    //   1     2
    //  / \   / \
    // 3   4 5   6
    // ...
    // parent       = (hole_index - 1) / 2 = (hole_index - 1) >> 1
    // first_child  = hole_index * 2 + 1   = (hole_index << 1) + 1
    // second_child = hole_index * 2 + 2   = (hole_index << 1) + 2

    //! Push Heap.

    template <typename RandomAccessIterator, typename Distance, typename T>
    void __push_heap(RandomAccessIterator first, Distance hole_index, Distance top_index, T value) {
        Distance parent = (hole_index - 1) / 2;
        while (hole_index > top_index && *(first + parent) < value) {
            // Put the smaller value at the child hole.
            *(first + hole_index) = *(first + parent);
            hole_index = parent;
            parent = (hole_index - 1) / 2;
        }
        // Put the value at the hole it stopped.
        *(first + hole_index) = value;
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
        __push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
    }

    // Make the elements in [`first`, `last`) a maximum heap.
    template <typename RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __push_heap_aux(first, last, difference_type(first), value_type(first));
    }

    template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
    void __push_heap(RandomAccessIterator first, Distance hole_index, Distance top_index, T value, Compare comp) {
        Distance parent = (hole_index - 1) / 2;
        while (hole_index > top_index && comp(*(first + parent), value)) {
            *(first + hole_index) = *(first + parent);
            hole_index = parent;
            parent = (hole_index - 1) / 2;
        }
        *(first + hole_index) = value;
    }

    template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
    inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
        __push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)), comp);
    }

    template <typename RandomAccessIterator, typename Compare>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        __push_heap_aux(first, last, difference_type(first), value_type(first), comp);
    }

    //! Push Heap.

    //! Pop Heap.

    template <typename RandomAccessIterator, typename Distance, typename T>
    void __adjust_heap(RandomAccessIterator first, Distance hole_index, Distance len, T value) {
        // Pop top element.
        Distance top_index = hole_index;
        Distance secondChild = 2 * hole_index + 2;
        while (secondChild < len) {
            // Swap with the bigger child.
            if (*(first + secondChild) < *(first + (secondChild - 1))) {
                // Become first child.
                secondChild--;
            }
            *(first + hole_index) = *(first + secondChild);
            hole_index = secondChild;
            secondChild = 2 * secondChild + 2;
        }
        // If reach the end, which means that `value` is still the minimum element.
        if (secondChild == len) {
            // Move the left child to parent hole.
            *(first + hole_index) = *(first + (secondChild - 1));
            hole_index = secondChild - 1;
        }
        // Insert the `value` at the hole it stopped.
        __push_heap(first, hole_index, top_index, value);
    }

    template <typename RandomAccessIterator, typename Distance, typename T>
    inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Distance*, T value) {
        // Put the top element at the end.
        *result = *first;
        // Pop top and insert at the last position.
        __adjust_heap(first, Distance(0), Distance(last - first), value);
    }

    template <typename RandomAccessIterator, typename T>
    inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
        __pop_heap(first, last - 1, last - 1, difference_type(first), T(*(last - 1)));
    }

    template <typename RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __pop_heap_aux(first, last, value_type(first));
    }

    template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
    void __adjust_heap(RandomAccessIterator first, Distance hole_index, Distance len, T value, Compare comp) {
        Distance top_index = hole_index;
        Distance secondChild = 2 * hole_index + 2;
        while (secondChild < len) {
            if (comp(*(first + secondChild), *(first + (secondChild - 1)))) {
                secondChild--;
            }
            *(first + hole_index) = *(first + secondChild);
            hole_index = secondChild;
            secondChild = 2 * secondChild + 2;
        }
        if (secondChild == len) {
            *(first + hole_index) = *(first + (secondChild - 1));
            hole_index = secondChild - 1;
        }
        __push_heap(first, hole_index, top_index, value, comp);
    }

    template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
    inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Distance*, T value, Compare comp) {
        *result = *first;
        __adjust_heap(first, Distance(0), Distance(last - first), value, comp);
    }

    template <typename RandomAccessIterator, typename Compare, typename T>
    inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp) {
        __pop_heap(first, last - 1, last - 1, difference_type(first), T(*(last - 1)), comp);
    }

    template <typename RandomAccessIterator, typename Compare>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        __pop_heap_aux(first, last, value_type(first), comp);
    }

    //! Pop Heap.

    //! Make Heap.

    template <typename RandomAccessIterator, typename Distance, typename T>
    void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
        if ((last - first) < 2) {
            return;
        }

        Distance len = last - first;
        Distance parent = (len - 2) / 2;

        // Floyd Algorithm.
        while (true) {
            __adjust_heap(first, parent, len, *(first + parent));
            if (parent == 0) {
                return;
            }
            --parent;
        }
    }

    template <typename RandomAccessIterator>
    inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
        __make_heap(first, last, difference_type(first), value_type(first));
    }

    template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
    void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
        if ((last - first) < 2) {
            return;
        }

        Distance len = last - first;
        Distance parent = (len - 2) / 2;

        while (true) {
            __adjust_heap(first, parent, len, *(first + parent), comp);
            if (parent == 0) {
                return;
            }
            --parent;
        }
    }

    template <typename RandomAccessIterator, typename Compare>
    inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        __make_heap(first, last, difference_type(first), value_type(first), comp);
    }

    //! Make Heap.

    //! Sort Heap.

    template <typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
        while (last - first > 1) {
            TinySTL::pop_heap(first, last);
            --last;
        }
    }

    template <typename RandomAccessIterator, typename Compare>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        while (last - first > 1) {
            TinySTL::pop_heap(first, last, comp);
            --last;
        }
    }

    //! Sort Heap.

    template <typename RandomAccessIterator, typename Distance>
    bool __is_heap(RandomAccessIterator first, Distance n) {
        Distance parent = 0;
        // Compare with its child.
        for (Distance child = 1; child < n; ++child) {
            if (*(first + parent) < *(first + child)) {
                return false;
            }
            // The second child is evenly indexed.
            if ((child & 1) == 0) {
                ++parent;
            }
        }
        return true;
    }

    template <typename RandomAccessIterator, typename Compare, typename Distance>
    bool __is_heap(RandomAccessIterator first, Distance n, Compare comp) {
        Distance parent = 0;
        for (Distance child = 1; child < n; ++child) {
            if (comp(*(first + parent), *(first + child))) {
                return false;
            }
            if ((child & 1) == 0) {
                ++parent;
            }
        }
        return true;
    }

    template <typename RandomAccessIterator>
    bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
        return __is_heap(first, last - first);
    }

    template <typename RandomAccessIterator, typename Compare>
    bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        return __is_heap(first, last - first, comp);
    }

} // namespace TinySTL

#endif // !_TINYSTL_HEAP_HPP_
