#ifndef _TINYSTL_ALLOCATOR_HPP_
#define _TINYSTL_ALLOCATOR_HPP_

#include <climits>
#include <new>
#include <stl_alloc.hpp>
#include <stl_construct.hpp>

namespace TinySTL {

    // Standard allocator but less used for being inefficient.
    template <typename T>
    class allocator {
    public:
        using value_type      = T;
        using pointer         = T*;
        using const_pointer   = const T*;
        using reference       = T&;
        using const_reference = const T&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;

        // Rebind allocator of type U.
        template <typename U>
        struct rebind {
            typedef allocator<U> other;
        };

    public:
        allocator() = default;

        allocator(const allocator&) = default;

        template <typename U>
        allocator(const allocator<U>&) {}

        ~allocator() = default;

        // Return the address of something.
        pointer address(reference value) const { return (pointer)&value; }
        // Return the address of a const thing.
        const_pointer address(const_reference value) const { return (const_pointer)&value; }
        // Maximum number of objects in init_page.
        size_type init_page_size() const { return std::max(size_type(1), size_type(4096 / sizeof(T))); }
        // Maximum number of objects.
        size_type max_size() const { return std::max(size_type(1), size_type(std::numeric_limits<size_t>::max() / sizeof(T))); }

        // Allocate spaces for one type T object.
        static pointer allocate() { return static_cast<pointer>(alloc::allocate(sizeof(T))); }
        // Allocate spaces for `n` type T objects.
        static pointer allocate(size_type n) {
            if (n == 0)
                return nullptr;
            return static_cast<pointer>(alloc::allocate(sizeof(T) * n));
        }
        // Deallocate space for one type T object.
        static void deallocate(pointer ptr) { alloc::deallocate(static_cast<void*>(ptr), sizeof(T)); }
        // Deallocate space for `n` type T objects.
        static void deallocate(pointer ptr, size_type n) {
            if (n == 0)
                return;
            alloc::deallocate(static_cast<void*>(ptr), sizeof(T) * n);
        }

        // Construct a type T object at `ptr`.
        static void construct(pointer ptr) { new (ptr) T(); }
        // Construct a type T object at `ptr` with given `value`.
        static void construct(pointer ptr, const_reference value) { new (ptr) T(value); }
        // Destroy a type T object at `ptr`.
        static void destroy(pointer ptr) { ptr->~T(); }
        // Destroy `n` type T objects from `first` to `last`.
        static void destroy(pointer first, pointer last) {
            for (; first != last; first++) {
                first->~T();
            }
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_ALLOCATOR_HPP_
