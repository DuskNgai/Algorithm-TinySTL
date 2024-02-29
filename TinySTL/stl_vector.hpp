#ifndef _TINYSTL_VECTOR_HPP_
#define _TINYSTL_VECTOR_HPP_

#include <stl_algorithm.hpp>
#include <stl_allocator.hpp>
#include <stl_iterator.hpp>
#include <stl_type_traits.hpp>
#include <stl_uninitialized.hpp>

namespace TinySTL {

    template <typename T, typename Alloc = allocator<T>>
    class vector {
    public:
        using value_type       = T;
        using pointer          = T*;
        using iterator         = T*;
        using reference        = T&;
        using size_type        = std::size_t;
        using difference_type  = std::ptrdiff_t;
        using const_pointer    = const T*;
        using const_iterator   = const T*;
        using const_reference  = const T&;
        using vector_allocator = simple_alloc<T, Alloc>;

    protected:
        // m_start position of used
        iterator m_start;
        // end position of used
        iterator m_finish;
        // end position of all space
        iterator m_end_of_storage;

    protected:
        /// @brief: Insert one `value` at given `pos`.
        void insert_aux(iterator pos, const T& value) {
            // If there are some space left.
            if (m_finish != m_end_of_storage) {
                // Copy the value in `m_finish - 1` to `m_finish`, it need to be constructed first.
                construct(m_finish, *(m_finish - 1));
                // Increment.
                ++m_finish;
                // Copy the value in [`pos`, `m_finish - 1`) to [`pos + 1`, `m_finish`).
                copy_backward(pos, m_finish - 2, m_finish - 1);
                // Put the inserted value.
                *pos = value;
            }
            // If there is no space left.
            else {
                // Allocate new space.
                const size_type old_capacity = capacity();
                const size_type new_capacity = old_capacity != 0 ? 2 * old_capacity : 1;
                iterator new_start           = vector_allocator::allocate(new_capacity);
                iterator new_finish          = new_start;
                try {
                    // Copy old [`m_start`, `pos`) to [`new_start`, `pos`).
                    new_finish = TinySTL::uninitialized_copy(m_start, pos, new_start);
                    // Put the inserted value.
                    construct(new_finish, value);
                    // Increment.
                    ++new_finish;
                    // copy old [`pos`, `m_finish`) to new space starting from `pos + 1`, a.k.a `new_finish`.
                    new_finish = TinySTL::uninitialized_copy(pos, m_finish, new_finish);
                }
                catch (const std::exception&) {
                    // If fail to construct, destroy and deallocate.
                    destory(new_start, new_finish);
                    vector_allocator::deallocate(new_start, new_capacity);
                    throw;
                }
                // Destroy and deallocation old space.
                destory(begin(), end());
                vector_allocator::deallocate(m_start, old_capacity);
                // Update the iterator.
                m_start          = new_start;
                m_finish         = new_finish;
                m_end_of_storage = new_start + new_capacity;
            }
        }

        /// @brief: Insert one default value at given `pos`.
        void insert_aux(iterator pos) {
            // If there are some space left.
            if (m_finish != m_end_of_storage) {
                // Copy the value in `m_finish - 1` to `m_finish`, it need to be constructed first.
                construct(m_finish, *(m_finish - 1));
                // Increment.
                ++m_finish;
                // Copy the value in [`pos`, `m_finish - 1`) to [`pos + 1`, `m_finish`).
                copy_backward(pos, m_finish - 2, m_finish - 1);
                // Put the inserted value.
                *pos = value_type();
            }
            // If there is no space left.
            else {
                // Allocate new space.
                const size_type old_capacity = capacity();
                const size_type new_capacity = old_capacity != 0 ? 2 * old_capacity : 1;
                iterator new_start           = vector_allocator::allocate(new_capacity);
                iterator new_finish          = new_start;
                try {
                    // Copy old [`m_start`, `pos`) to [`new_start`, `pos`).
                    new_finish = TinySTL::uninitialized_copy(m_start, pos, new_start);
                    // Put the inserted value.
                    construct(new_finish);
                    // Increment.
                    ++new_finish;
                    // copy old [`pos`, `m_finish`) to space starting from `pos + 1`, a.k.a `new_finish`.
                    new_finish = TinySTL::uninitialized_copy(pos, m_finish, new_finish);
                }
                catch (const std::exception&) {
                    // If fail to construct, destroy and deallocate.
                    destory(new_start, new_finish);
                    vector_allocator::deallocate(new_start, new_capacity);
                    throw;
                }
                // Destroy and deallocation old space.
                destory(begin(), end());
                vector_allocator::deallocate(m_start, old_capacity);
                // Update the iterator.
                m_start          = new_start;
                m_finish         = new_finish;
                m_end_of_storage = new_start + new_capacity;
            }
        }

    public:
        vector()
            : m_start(nullptr), m_finish(nullptr), m_end_of_storage(nullptr) {}

        vector(size_type n, const T& value = value_type()) {
            m_start          = vector_allocator::allocate(n);
            m_finish         = TinySTL::uninitialized_fill_n(begin(), n, value);
            m_end_of_storage = m_finish;
        }

        vector(const vector& other) {
            m_start          = vector_allocator::allocate(other.size());
            m_finish         = TinySTL::uninitialized_copy(other.begin(), other.end(), begin());
            m_end_of_storage = m_finish;
        }

        vector(const value_type* first, const value_type* last) {
            m_start          = vector_allocator::allocate(distance(first, last));
            m_finish         = TinySTL::uninitialized_copy(first, last, begin());
            m_end_of_storage = m_finish;
        }

        vector(const iterator first, const iterator last) {
            m_start          = vector_allocator::allocate(distance(first, last));
            m_finish         = TinySTL::uninitialized_copy(first, last, begin());
            m_end_of_storage = m_finish;
        }

        vector& operator=(const vector& other) {
            if (this != &other) {
                // Destroy and deallocation old space.
                destory(begin(), end());
                vector_allocator::deallocate(begin(), capacity());
                // Allocate new space.
                m_start          = vector_allocator::allocate(other.size());
                m_finish         = TinySTL::uninitialized_copy(other.begin(), other.end(), begin());
                m_end_of_storage = m_finish;
            }
            return *this;
        }

        ~vector() {
            // Destroy and deallocation old space.
            destory(m_start, m_finish);
            vector_allocator::deallocate(m_start, capacity());
            // Reset the iterator.
            m_start          = nullptr;
            m_finish         = nullptr;
            m_end_of_storage = nullptr;
        }

    public:
        iterator begin() { return m_start; }
        iterator end() { return m_finish; }
        const_iterator begin() const { return m_start; }
        const_iterator end() const { return m_finish; }
        const_iterator cbegin() const { return m_start; }
        const_iterator cend() const { return m_finish; }

        size_type size() const { return (size_type)(end() - begin()); }
        size_type capacity() const { return (size_type)(m_end_of_storage - begin()); }
        bool empty() const { return begin() == end(); }

        reference front() { return *begin(); }
        reference back() { return *(end() - 1); }
        reference operator[](size_type idx) { return *(begin() + idx); }
        const_reference front() const { return *begin(); }
        const_reference back() const { return *(end() - 1); }
        const_reference operator[](size_type idx) const { return *(begin() + idx); }

    public:
        /// @brief Add a `value` at the back of the vector.
        void push_back(const T& value) {
            if (m_finish != m_end_of_storage) {
                construct(m_finish, value);
                ++m_finish;
            }
            else {
                insert_aux(end(), value);
            }
        }

        /// @brief Add a default value at the back of the vector.
        void push_back() {
            if (m_finish != m_end_of_storage) {
                construct(m_finish);
                ++m_finish;
            }
            else {
                insert_aux(end());
            }
        }

        void pop_back() {
            --m_finish;
            destory(m_finish);
        }

        iterator insert(iterator pos, const T& value) {
            size_type n = pos - begin();
            // Insert at `m_finish`.
            if (m_finish != m_end_of_storage && pos == end()) {
                construct(m_finish, value);
                ++m_finish;
            }
            // Insert at middle.
            else {
                insert_aux(pos, value);
            }
            return begin() + n;
        }

        iterator insert(iterator pos) {
            size_type n = pos - begin();
            // Insert at `m_finish`.
            if (m_finish != m_end_of_storage && pos == end()) {
                construct(m_finish);
                ++m_finish;
            }
            // Insert at middle.
            else {
                insert_aux(pos);
            }
            return begin() + n;
        }

        void insert(iterator pos, size_type n, const T& value) {
            if (n != 0) {
                // If there is enough space.
                if ((size_type)(m_end_of_storage - m_finish) >= n) {
                    const size_type n_elems_after = m_finish - pos;
                    iterator old_finish           = m_finish;
                    // If there are more elements after `pos` than number of inserted values.
                    if (n_elems_after > n) {
                        // Copy [`m_finish - n`, `m_finish`) to [`m_finish`, `m_finish + n`).
                        m_finish = TinySTL::uninitialized_copy(m_finish - n, m_finish, m_finish);
                        // Copy [`pos`, `m_finish - n`) to [`pos + n`, `m_finish`).
                        TinySTL::copy_backward(pos, old_finish - n, old_finish);
                        // Fill [`pos`, `pos + n`) with `value`.
                        TinySTL::fill(pos, pos + n, value);
                    }
                    // Otherwise, more inserted values.
                    else {
                        // Fill [`m_finish`, `pos + n`) with `value`.
                        m_finish = TinySTL::uninitialized_fill_n(m_finish, n - n_elems_after, value);
                        // Copy [`pos`, `m_finish`) to [`pos + n`, `m_finish + n`).
                        m_finish = TinySTL::uninitialized_copy(pos, old_finish, m_finish);
                        // Fill [`pos`, `m_finish`) with `value`.
                        TinySTL::fill(pos, old_finish, value);
                    }
                }
                // If there is not enough space.
                else {
                    // First allocate space.
                    const size_type old_size     = size();
                    const size_type old_capacity = capacity();
                    const size_type new_capacity = old_size + n;
                    iterator new_start           = vector_allocator::allocate(new_capacity);
                    iterator new_finish          = new_start;
                    try {
                        // Copy old [`m_start`, `pos`) to new space starting from `new_start`.
                        new_finish = TinySTL::uninitialized_copy(m_start, pos, new_start);
                        // Put n inserted values.
                        new_finish = TinySTL::uninitialized_fill_n(new_finish, n, value);
                        // Copy old [`pos`, `m_finish`) to new space starting from `pos + n`.
                        new_finish = TinySTL::uninitialized_copy(pos, m_finish, new_finish);
                    }
                    catch (const std::exception&) {
                        destory(new_start, new_finish);
                        vector_allocator::deallocate(new_start, new_capacity);
                        throw;
                    }
                    // Destroy and deallocation old space.
                    destory(begin(), end());
                    vector_allocator::deallocate(m_start, old_capacity);
                    // Update the iterator.
                    m_start          = new_start;
                    m_finish         = new_finish;
                    m_end_of_storage = new_start + new_capacity;
                }
            }
        }

        template <typename InputIterator>
        void insert(iterator pos, InputIterator first, InputIterator last) {
            size_type n = TinySTL::distance(first, last);
            if (n != 0) {
                // If there is enough space.
                if ((size_type)(m_end_of_storage - m_finish) >= n) {
                    const size_type n_elems_after = m_finish - pos;
                    iterator old_finish           = m_finish;
                    // If there are more elements after `pos` than number of inserted values.
                    if (n_elems_after > n) {
                        // Copy [`m_finish - n`, `m_finish`) to [`m_finish`, `m_finish + n`).
                        m_finish = TinySTL::uninitialized_copy(m_finish - n, m_finish, m_finish);
                        // Copy [`pos`, `m_finish - n`) to [`pos + n`, `m_finish`).
                        TinySTL::copy_backward(pos, old_finish - n, old_finish);
                        // Copy [`first`, `last`) to [`pos`, `pos + n`).
                        TinySTL::copy(first, last, pos);
                    }
                    // Otherwise, more inserted values.
                    else {
                        // Copy [`first + n_elems_after`, `last`) to [`m_finish`, `pos + n`).
                        m_finish = TinySTL::uninitialized_copy(first + n_elems_after, last, m_finish);
                        // Copy [`pos`, `m_finish`) to [`pos + n`, `m_finish + n`).
                        m_finish = TinySTL::uninitialized_copy(pos, old_finish, m_finish);
                        // Copy [`first`, `first + n_elems_after`) to [`pos`, `m_finish`).
                        TinySTL::copy(first, first + n_elems_after, pos);
                    }
                }
                // If there is not enough space.
                else {
                    // First allocate space.
                    const size_type old_size     = size();
                    const size_type old_capacity = capacity();
                    const size_type new_capacity = old_size + n;
                    iterator new_start           = vector_allocator::allocate(new_capacity);
                    iterator new_finish          = new_start;
                    try {
                        // Copy old [`m_start`, `pos`) to new space starting from `new_start`.
                        new_finish = TinySTL::uninitialized_copy(m_start, pos, new_start);
                        // Copy old [`first`, `last`) to new space starting from `pos`.
                        new_finish = TinySTL::uninitialized_copy(first, last, new_finish);
                        // Copy old [`pos`, `m_finish`) to new space starting from `pos + n`.
                        new_finish = TinySTL::uninitialized_copy(pos, m_finish, new_finish);
                    }
                    catch (const std::exception&) {
                        destory(new_start, new_finish);
                        vector_allocator::deallocate(new_start, new_capacity);
                        throw;
                    }
                    // Destroy and deallocation old space.
                    destory(begin(), end());
                    vector_allocator::deallocate(m_start, old_capacity);
                    // Update the iterator.
                    m_start          = new_start;
                    m_finish         = new_finish;
                    m_end_of_storage = new_start + new_capacity;
                }
            }
        }

        iterator erase(iterator pos) {
            if (pos + 1 != end()) {
                copy(pos + 1, m_finish, pos);
            }
            pop_back();
            return pos;
        }

        iterator erase(iterator first, iterator last) {
            // Copy [`last`, `m_finish`) to [`first`, `m_finish - (last - first)`).
            iterator new_finish = TinySTL::copy(last, m_finish, first);
            destory(new_finish, m_finish);
            m_finish = new_finish;
            return first;
        }

        void clear() {
            erase(begin(), end());
        }

        void resize(size_type new_size, const T& value = value_type()) {
            if (new_size < size()) {
                erase(begin() + new_size, end());
            }
            else {
                insert(end(), new_size - size(), value);
            }
        }

        void reserve(size_type n) {
            if (capacity() < n) {
                // Allocate new space.
                const size_type old_capacity = capacity();
                iterator new_start           = vector_allocator::allocate(n);
                // Copy old space to new space.
                iterator new_finish = uninitialized_copy(begin(), end(), new_start);
                // Destroy and deallocate old space.
                destory(m_start, m_finish);
                vector_allocator::deallocate(m_start, old_capacity);
                // Update the iterator.
                m_start          = new_start;
                m_finish         = new_finish;
                m_end_of_storage = new_start + n;
            }
        }

        void shrink_to_fit() {
            if (size() != capacity()) {
                // Allocate new space.
                iterator new_start = vector_allocator::allocate(size());
                // Copy old space to new space.
                iterator new_finish = TinySTL::uninitialized_copy(begin(), end(), new_start);
                destory(m_start, m_finish);
                vector_allocator::deallocate(m_start, capacity());
                // Update the iterator.
                m_start          = new_start;
                m_finish         = new_finish;
                m_end_of_storage = new_finish;
            }
        }

        void swap(vector& other) {
            TinySTL::swap(m_start, other.m_start);
            TinySTL::swap(m_finish, other.m_finish);
            TinySTL::swap(m_end_of_storage, other.m_end_of_storage);
        }

        bool operator==(const vector& other) {
            return size() == other.size() && equal(begin(), end(), other.begin());
        }

        bool operator!=(const vector& other) {
            return !operator==(other);
        }

        bool operator<(const vector& other) {
            return TinySTL::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_VECTOR_HPP_
