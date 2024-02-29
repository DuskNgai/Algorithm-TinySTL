#ifndef _TINYSTL_UNINITIALIZED_HPP_
#define _TINYSTL_UNINITIALIZED_HPP_

#include <stl_algorithm.hpp>
#include <stl_construct.hpp>
#include <stl_type_traits.hpp>

namespace TinySTL {

    //! uninitialized_copy.

    /// @brief If the type is POD type, copy the data is enough.
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
        return copy(first, last, result);
    }

    /// @brief If the type is not POD type, construct one by one.
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
        for (; first != last; ++first, ++result) {
            construct(&*result, *first);
        }
        return result;
    }

    /// @brief Dispatch to auxilary function that based on whether it is POD type or not.
    template <typename InputIterator, typename ForwardIterator, typename T>
    ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T* p) {
        using is_POD = typename __type_traits<T>::is_POD_type;
        return _uninitialized_copy_aux(first, last, result, is_POD());
    }

    /// @brief Based on the type of the data, it will choose the copy or construct a new one.
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        return _uninitialized_copy(first, last, result, value_type(result));
    }

    //! uninitialized_copy.

    //! uninitialized_fill.

    /// @brief If the type is POD type, copy the data is enough.
    template <typename ForwardIterator, typename T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __true_type) {
        fill(first, last, value);
    }

    /// @brief If the type is not POD type, construct one by one.
    template <typename ForwardIterator, typename T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __false_type) {
        for (; first != last; ++first) {
            construct(&*first, value);
        }
    }

    /// @brief Dispatch to auxilary function that based on whether it is POD type or not.
    template <typename ForwardIterator, typename T, typename I>
    inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, I*) {
        using is_POD = typename __type_traits<I>::is_POD_type;
        _uninitialized_fill_aux(first, last, value, is_POD());
    }

    /// @brief Based on the type of the iterator (Since the type of value and that of iterator may be different), it will choose the fill or construct a new one.
    template <typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
        _uninitialized_fill(first, last, value, value_type(first));
    }

    //! uninitialized_fill.

    //! uninitialized_fill_n.

    /// @brief If the type is POD type, copy the data is enough.
    template <typename ForwardIterator, typename size, typename T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, size n, const T& value, __true_type) {
        return fill_n(first, n, value);
    }

    /// @brief If the type is not POD type, construct one by one.
    template <typename ForwardIterator, typename size, typename T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, size n, const T& value, __false_type) {
        for (; n > 0; --n, ++first)
            construct(&*first, value);
        return first;
    }

    /// @brief Dispatch to auxilary function that based on whether it is POD type or not.
    template <typename ForwardIterator, typename size, typename T, typename I>
    ForwardIterator _uninitialized_fill_n(ForwardIterator first, size n, const T& value, I* t) {
        using is_POD = typename __type_traits<I>::is_POD_type;
        return _uninitialized_fill_n_aux(first, n, value, is_POD());
    }

    /// @brief Based on the type of the iterator (Since the type of value and that of iterator may be different), it will choose the fill or construct n new one.
    template <typename ForwardIterator, typename size, typename T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, size n, const T& value) {
        return _uninitialized_fill_n(first, n, value, value_type(first));
    }

    //! uninitialized_fill_n.

} // namespace TinySTL

#endif // !_TINYSTL_UNINITIALIZED_HPP_
