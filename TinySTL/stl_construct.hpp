#ifndef _TINYSTL_CONSTRUCT_HPP_
#define _TINYSTL_CONSTRUCT_HPP_

#include <new>
#include <stl_type_traits.hpp>

namespace TinySTL {

    // In responsible for construct and destruct,
    // calling of function will be expanded in compile time.

    // Construct with value.
    template <typename T1, typename T2>
    inline void construct(T1* ptr, const T2& value) {
        new (ptr) T1(value);
    }

    // Construct without value.
    template <typename T>
    inline void construct(T* ptr) {
        new (ptr) T();
    }

    // Destruct one object.
    template <typename T>
    inline void destory(T* ptr) {
        ptr->~T();
    }

    // Need to be destructed.
    template <typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
        for (; first != last; first++)
            destory(&*first);
    }

    // Trivial destructor.
    template <typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}

    // Based on whether it has trivial destructor.
    template <typename ForwardIterator, typename T>
    inline void __destory(ForwardIterator first, ForwardIterator last, T*) {
        using trivial_destructor = typename __type_traits<T>::has_trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    // Destruct a range of objects.
    template <typename ForwardIterator>
    inline void destory(ForwardIterator first, ForwardIterator last) {
        __destory(first, last, value_type(first));
    }

    inline void destory(char*, char*) {}
    inline void destory(wchar_t*, wchar_t*) {}

} // namespace TinySTL

#endif // !_TINYSTL_CONSTRUCT_HPP_
