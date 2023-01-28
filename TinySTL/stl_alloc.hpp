#ifndef _TINYSTL_ALLOC_HPP_
#define _TINYSTL_ALLOC_HPP_

#include <cstdlib>
#include <iostream>
#include <new>
#include <stl_construct.hpp>

namespace TinySTL {

    // Primary allocator.
    template <int inst>
    class __malloc_alloc_template {
    private:
        // oom: out of memory
        // These are all function pointer.
        static void* __oom_malloc(size_t);
        static void* __oom_realloc(void*, size_t);
        static void (*__malloc_alloc_oom_handler)();

    public:
        static void* allocate(size_t n) {
            void* space = malloc(n);
            if (space == nullptr)
                space = __oom_malloc(n);
            return space;
        }

        static void deallocate(void* ptr, size_t n) {
            ::operator delete(ptr);
        }

        static void reallocate(void* ptr, size_t old_size, size_t new_size) {
            void* space = realloc(ptr, new_size);
            if (space == nullptr)
                space = __oom_realloc(ptr, new_size);
            return space;
        }

        static void (*__set_malloc_handler(void (*f)()))() {
            void (*old)()              = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return old;
        }
    };

    template <int inst>
    void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

    template <int inst>
    void* __malloc_alloc_template<inst>::__oom_malloc(size_t n) {
        void (*malloc_handler)();
        void* space;

        while (true) {
            malloc_handler = __malloc_alloc_oom_handler;
            if (malloc_handler == 0) {
                std::cerr << "Out of memory.\n";
                exit(1);
            }
            // Free.
            (*malloc_handler)();
            // Malloc again.
            space = malloc(n);
            if (space != nullptr)
                return space;
        }
    }

    template <int inst>
    void* __malloc_alloc_template<inst>::__oom_realloc(void* ptr, size_t n) {
        void (*malloc_handler)();
        void* space;

        while (true) {
            malloc_handler = __malloc_alloc_oom_handler;
            if (malloc_handler == 0) {
                std::cerr << "Out of memory.\n";
                exit(1);
            }
            // Free.
            (*malloc_handler)();
            // Malloc again.
            space = realloc(ptr, n);
            if (space != nullptr)
                return space;
        }
    }

    typedef __malloc_alloc_template<0> malloc_alloc;

    // Encapsule the `alloc` as an interface.
    template <class T, class Alloc>
    class simple_alloc {
    public:
        static T* allocate(size_t n) {
            return 0 == n ? nullptr : static_cast<T*>(Alloc::allocate(n * sizeof(T)));
        }
        static T* allocate(void) {
            return static_cast<T*>(Alloc::allocate(sizeof(T)));
        }
        static void deallocate(T* ptr, size_t n) {
            if (0 != n)
                Alloc::deallocate(ptr, n * sizeof(T));
        }
        static void deallocate(T* ptr) {
            Alloc::deallocate(ptr, sizeof(T));
        }
    };


    // Secondary allocator.
    // Apply internal interfaces for `stl_allocator`,
    // in responsible for memory allocation and deallocation.
    template <bool threads, int inst>
    class __default_alloc_template {
    private:
        // Small block should be the multiply of 8.
        enum { _ALIGN = 8 };
        // The max size of small block.
        enum { _MAX_BYTES = 128 };
        // Number of free lists.
        enum { _NFREELISTS = _MAX_BYTES / _ALIGN };

        // Round up to be the multiplicity of 8.
        static size_t Round_Up(size_t bytes) {
            return (((bytes) + _ALIGN - 1) & ~(_ALIGN - 1));
        }

    private:
        union obj {
            union obj* next;
            char client[1];
        };

        // 16 free lists.
        static obj* volatile free_list[_NFREELISTS];
        // Use the n^th free list, starting from 0.
        static size_t freelist_index(size_t bytes) {
            return (((bytes) + _ALIGN - 1) / _ALIGN) - 1;
        }

        // Return an object of size `n`,
        // and optionally adds to size `n` free list.
        static void* refill(size_t n);
        // allocates a chunk for nobjs of size "size".
        // nobjs may be reduced if it is inconvenient to
        // allocate the requested number.
        static char* chunk_alloc(size_t size, int& nobjs);

        // Chunk allocation state
        // start position of free list
        static char* start_free;
        // end position of free list
        static char* end_free;
        static size_t heap_size;

    public:
        static void* allocate(size_t bytes);

        static void deallocate(void* ptr, size_t n);

        static void* reallocate(void* ptr, size_t old_size, size_t new_size);
    };

    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::start_free = 0;

    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::end_free = 0;

    template <bool threads, int inst>
    size_t __default_alloc_template<threads, inst>::heap_size = 0;

    template <bool threads, int inst>
    typename __default_alloc_template<threads, inst>::obj* volatile __default_alloc_template<threads, inst>::free_list[_NFREELISTS] = {0};

    template <bool threads, int inst>
    void* __default_alloc_template<threads, inst>::allocate(size_t n) {
        // If more than 128, using primary allocator.
        if (n > (size_t)_MAX_BYTES) {
            return malloc_alloc::allocate(n);
        }

        obj* volatile* my_free_list = free_list + freelist_index(n);
        obj* space                  = *my_free_list;
        if (space == nullptr) {
            // No available free list.
            void* r = refill(Round_Up(n));
            return r;
        }
        // Adjust free list.
        *my_free_list = space->next;
        return space;
    }

    template <bool threads, int inst>
    void __default_alloc_template<threads, inst>::deallocate(void* ptr, size_t n) {
        // If more than 128, using primary deallocator.
        if (n > (size_t)_MAX_BYTES) {
            malloc_alloc::deallocate(ptr, n);
            return;
        }

        obj* volatile* my_free_list = free_list + freelist_index(n);
        obj* q                      = (obj*)ptr;
        q->next                     = *my_free_list;
        *my_free_list               = q;
    }

    template <bool threads, int inst>
    void* __default_alloc_template<threads, inst>::reallocate(void* ptr, size_t old_size, size_t new_size) {
        // If more than 128, using primary reallocator.
        if (old_size > (size_t)_MAX_BYTES && new_size > (size_t)_MAX_BYTES) {
            return realloc(ptr, new_size);
        }

        // If the size is the same, return the same pointer.
        if (Round_Up(old_size) == Round_Up(new_size)) {
            return ptr;
        }

        // If the size is different, allocate a new space and copy the data.
        void* result     = allocate(new_size);
        size_t copy_size = new_size > old_size ? old_size : new_size;
        memcpy(result, ptr, copy_size);
        deallocate(ptr, old_size);
        return result;
    }

    typedef __default_alloc_template<false, 0> alloc;

    template <bool threads, int inst>
    void* __default_alloc_template<threads, inst>::refill(size_t n) {
        int nobjs = 20;

        char* chunk = chunk_alloc(n, nobjs);
        // If get one chunk.
        if (nobjs == 1)
            return chunk;

        // Build free list in chunk.
        obj* next_obj;
        obj* space                  = (obj*)chunk;
        obj* volatile* my_free_list = free_list + freelist_index(n);
        *my_free_list = next_obj = (obj*)(chunk + n);
        for (int i = 1;; i++) {
            obj* current_obj = next_obj;
            next_obj         = (obj*)((char*)next_obj + n);
            if (nobjs - 1 == i) {
                current_obj->next = 0;
                break;
            }
            else {
                current_obj->next = next_obj;
            }
        }
        return space;
    }

    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs) {
        size_t total_bytes = size * nobjs;
        size_t bytes_left  = end_free - start_free;

        // If enough for all.
        if (bytes_left >= total_bytes) {
            char* result = start_free;
            start_free += total_bytes;
            return result;
        }
        // If not enough for all but at least one.
        else if (bytes_left >= size) {
            nobjs        = bytes_left / size;
            total_bytes  = size * nobjs;
            char* result = start_free;
            start_free += total_bytes;
            return result;
        }
        // If not enough for one.
        else {
            size_t bytes_to_get = 2 * total_bytes + Round_Up(heap_size >> 4);
            // Try to make use of the left-over piece.
            if (bytes_left > 0) {
                // Search for free list.
                obj* volatile* my_free_list = free_list + freelist_index(bytes_left);
                ((obj*)start_free)->next    = *my_free_list;
                *my_free_list               = (obj*)start_free;
            }

            // Config heap space for memory pool.
            start_free = (char*)malloc(bytes_to_get);
            if (start_free == NULL) {
                // Malloc fails.
                obj * volatile *my_free_list, *p;
                // Search for unused and big enough free list.
                for (int i = size; i <= _MAX_BYTES; i += _ALIGN) {
                    my_free_list = free_list + freelist_index(i);
                    p            = *my_free_list;
                    if (p != NULL) {
                        // Adjust free list.
                        *my_free_list = p->next;
                        start_free    = (char*)p;
                        end_free      = start_free + i;
                        // Adjust nobjs.
                        return chunk_alloc(size, nobjs);
                    }
                }
                // No any space.
                end_free = NULL;
                // This will throw exception.
                start_free = (char*)malloc_alloc::allocate(bytes_to_get);
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }

} // namespace TinySTL

#endif // !_TINYSTL_ALLOC_HPP_
