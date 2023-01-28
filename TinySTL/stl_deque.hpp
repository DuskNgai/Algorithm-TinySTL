#ifndef _TINYSTL_DEQUE_HPP_
#define _TINYSTL_DEQUE_HPP_

#include <stl_algorithm.hpp>
#include <stl_allocator.hpp>
#include <stl_iterator.hpp>
#include <stl_type_traits.hpp>
#include <stl_uninitialized.hpp>

namespace TinySTL {

    constexpr size_t deque_buffer_size(size_t n, size_t size) {
        return n != 0 ? n : (size < 512 ? size_t(512 / size) : size_t(1));
    }

    template <typename T, typename Ref, typename Ptr, size_t BufSize>
    struct deque_iterator {
        static constexpr size_t buffer_size() {
            return deque_buffer_size(BufSize, sizeof(T));
        }

        using iterator       = deque_iterator<T, T&, T*, BufSize>;
        using const_iterator = deque_iterator<T, const T&, const T*, BufSize>;

        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using pointer           = Ptr;
        using reference         = Ref;
        using size_type         = size_t;
        using difference_type   = ptrdiff_t;

        using map_pointer = T**;
        using self        = deque_iterator<T, Ref, Ptr, BufSize>;

        // Current buffer, current position.
        pointer m_curr;
        // Current buffer, current buffer head.
        pointer m_first;
        // Current buffer, current buffer tail.
        pointer m_last;
        // Current buffer pointer.
        map_pointer m_node;

        deque_iterator()
            : m_curr(nullptr), m_first(nullptr), m_last(nullptr), m_node(nullptr) {}
        deque_iterator(T* pos, map_pointer map)
            : m_curr(pos), m_first(*map), m_last(*map + buffer_size()), m_node(map) {}
        deque_iterator(const iterator& other)
            : m_curr(other.m_curr), m_first(other.m_first), m_last(other.m_last), m_node(other.m_node) {}

        reference operator*() const { return *m_curr; }
        pointer operator->() const { return m_curr; }

        difference_type operator-(const self& other) const {
            return difference_type(buffer_size()) * (m_node - other.m_node - 1) +
                   (m_curr - m_first) + (other.m_last - other.m_curr);
        }

        void set_node(map_pointer new_node) {
            m_first = *new_node;
            m_last  = *new_node + difference_type(buffer_size());
            m_node  = new_node;
        }

        deque_iterator& operator++() {
            ++m_curr;
            if (m_curr == m_last) {
                set_node(m_node + 1);
                m_curr = m_first;
            }
            return *this;
        }

        deque_iterator operator++(int) {
            deque_iterator temp = *this;
            ++*this;
            return temp;
        }

        deque_iterator& operator--() {
            if (m_curr == m_first) {
                set_node(m_node - 1);
                m_curr = m_last;
            }
            --m_curr;
            return *this;
        }

        deque_iterator operator--(int) {
            deque_iterator temp = *this;
            --*this;
            return temp;
        }

        deque_iterator& operator+=(difference_type n) {
            difference_type pos_offset = n + (m_curr - m_first);
            // If it is still in the current buffer.
            // If `pos_offset` is in [0, buffer_size()) of `m_first`.
            if (pos_offset >= 0 && pos_offset < difference_type(buffer_size())) {
                m_curr += n;
            }
            // If across other buffers.
            else {
                // If in [buffer_size(), +infty), divide directively.
                // If in (-infty, 0), move to correspond buffer's head.
                difference_type node_offset =
                    pos_offset > 0 ? pos_offset / difference_type(buffer_size())
                                   : -difference_type((-pos_offset - 1) / buffer_size()) - 1;
                set_node(m_node + node_offset);
                // Then adjust the `m_curr`.
                m_curr = m_first + (pos_offset - node_offset * difference_type(buffer_size()));
            }
            return *this;
        }

        deque_iterator operator+(difference_type n) {
            deque_iterator temp = *this;
            return temp += n;
        }

        deque_iterator& operator-=(difference_type n) {
            return *this += (-n);
        }

        deque_iterator operator-(difference_type n) {
            deque_iterator temp = *this;
            return temp -= n;
        }

        reference operator[](difference_type n) const {
            return *((*this + n));
        }

        bool operator==(const deque_iterator& other) const {
            return m_curr == other.m_curr;
        }

        bool operator!=(const deque_iterator& other) const {
            return m_curr != other.m_curr;
        }

        bool operator<(const deque_iterator& other) const {
            return (m_node == other.m_node) ? (m_curr < other.m_curr) : (m_node < other.m_node);
        }
    };

    template <typename T, typename Ref, typename Ptr, size_t BufSize>
    constexpr ptrdiff_t* distance_type(const deque_iterator<T, Ref, Ptr, BufSize>&) {
        return static_cast<ptrdiff_t*>(0);
    }

    template <typename T, typename Ref, typename Ptr, size_t BufSize>
    constexpr T* value_type(const deque_iterator<T, Ref, Ptr, BufSize>&) {
        return static_cast<T*>(0);
    }

    template <typename T, typename Ref, typename Ptr, size_t BufSize>
    constexpr random_access_iterator_tag iterator_tag(const deque_iterator<T, Ref, Ptr, BufSize>&) {
        return random_access_iterator_tag();
    }

    template <typename T, typename Alloc = alloc, size_t BufSize = 0>
    class deque {
    public:
        using value_type      = T;
        using pointer         = T*;
        using reference       = T&;
        using const_pointer   = const T*;
        using const_reference = const T&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;

        using iterator       = deque_iterator<T, T&, T*, BufSize>;
        using const_iterator = deque_iterator<T, const T&, const T*, BufSize>;

    protected:
        using node_allocator = simple_alloc<T, Alloc>;
        using map_allocator  = simple_alloc<T*, Alloc>;
        using map_pointer    = pointer*;

        iterator m_start;
        iterator m_finish;
        size_type m_map_size;
        map_pointer m_map;

        enum class MapSize { MAP_SIZE = 8 };

    public:
        map_pointer allocate_map(size_t n) { return map_allocator::allocate(n); }

        void deallocate_map(map_pointer map_ptr, size_t n) { map_allocator::deallocate(map_ptr, n); }

        void reallocate_map(size_type node_to_add, bool add_at_front) {
            size_type old_num_nodes = m_finish.m_node - m_start.m_node + 1;
            size_type new_num_nodes = old_num_nodes + node_to_add;

            map_pointer new_start_node;
            // If there is enough space, just move the start and finish.
            if (m_map_size > 2 * new_num_nodes) {
                new_start_node = m_map + (m_map_size - new_num_nodes) / 2 + (add_at_front ? node_to_add : 0);
                // If the `m_start` node changed.
                if (new_start_node < m_start.m_node) {
                    // Copy [`m_start.m_first`, `m_finish.m_last`) to [`new_start_node.m_first`, `new_start_node + old_num_nodes.m_last`).
                    copy(m_start.m_node, m_finish.m_node + 1, new_start_node);
                }
                // If the `m_finish` node changed.
                else {
                    // Copy [`m_start.m_first`, `m_finish.m_last`) to [`new_start_node.m_first`, `new_start_node + old_num_nodes.m_last`).
                    copy_backward(m_start.m_node, m_finish.m_node + 1, new_start_node + old_num_nodes);
                }
            }
            else {
                // Allocate a new map.
                size_type new_map_size = m_map_size + std::max(m_map_size, node_to_add) + 2;
                map_pointer new_map    = allocate_map(new_map_size);
                new_start_node         = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? node_to_add : 0);
                // Copy [`m_start.m_first`, `m_finish.m_last`) to [`new_start_node.m_first`, `new_start_node + old_num_nodes.m_last`).
                copy(m_start.m_node, m_finish.m_node + 1, new_start_node);
                deallocate_map(m_map, m_map_size);
                m_map      = new_map;
                m_map_size = new_map_size;
            }

            m_start.set_node(new_start_node);
            m_finish.set_node(new_start_node + old_num_nodes - 1);
        }

        void initialize_map(size_type num_elements) {
            // First allocate a map.
            size_type num_nodes = num_elements / deque_buffer_size(BufSize, sizeof(T)) + 1;
            m_map_size          = std::max((size_type)(MapSize::MAP_SIZE), num_nodes + 2);
            m_map               = allocate_map(m_map_size);

            map_pointer start_node  = m_map + (m_map_size - num_nodes) / 2;
            map_pointer finish_node = start_node + num_nodes;

            try {
                create_nodes(start_node, finish_node);
            }
            catch (const std::exception&) {
                deallocate_map(m_map, m_map_size);
                m_map      = nullptr;
                m_map_size = 0;
                throw;
            }

            m_start.set_node(start_node);
            m_start.m_curr = m_start.m_first;
            m_finish.set_node(finish_node - 1);
            m_finish.m_curr = m_finish.m_first + num_elements % deque_buffer_size(BufSize, sizeof(T));
        }

        pointer allocate_node() {
            return node_allocator::allocate(deque_buffer_size(BufSize, sizeof(T)));
        }

        void deallocate_node(pointer node) {
            node_allocator::deallocate(node, deque_buffer_size(BufSize, sizeof(T)));
        }

        void create_nodes(map_pointer start_node, map_pointer finish_node) {
            map_pointer curr_node = start_node;
            try {
                // Allocate each node.
                for (; curr_node != finish_node; ++curr_node) {
                    *curr_node = allocate_node();
                }
            }
            catch (const std::exception&) {
                destroy_nodes(start_node, curr_node);
                throw;
            }
        }

        void destroy_nodes(map_pointer start_node, map_pointer finish_node) {
            // Deallocate each node.
            for (map_pointer curr_node = start_node; curr_node != finish_node; ++curr_node) {
                deallocate_node(*curr_node);
            }
        }

        void fill_initialize(const value_type& value) {
            map_pointer curr_node = m_start.m_node;
            try {
                // Fill each node.
                for (; curr_node < m_finish.m_node; ++curr_node) {
                    uninitialized_fill(*curr_node, *curr_node + deque_buffer_size(BufSize, sizeof(T)), value);
                }
                // Fill the last node.
                uninitialized_fill(m_finish.m_first, m_finish.m_curr, value);
            }
            catch (const std::exception&) {
                destroy(m_start, iterator(*curr_node, curr_node));
                throw;
            }
        }

        iterator reserve_element_at_front(size_type n) {
            // Calculate the vacancy of the first buffer.
            size_type vacancy = m_start.m_curr - m_start.m_first;
            // If not enough space at front of the first buffer.
            if (n > vacancy) {
                // Allocate new buffers.
                new_elements_at_front(n - vacancy);
            }
            return m_start - difference_type(n);
        }

        iterator reserve_element_at_back(size_type n) {
            // Calculate the vacancy of the last buffer.
            size_type vacancy = (m_finish.m_last - m_finish.m_curr) - 1;
            // If not enough space at back of the last buffer.
            if (n > vacancy) {
                // Allocate new buffers.
                new_elements_at_back(n - vacancy);
            }
            return m_finish + difference_type(n);
        }

        void new_elements_at_front(size_type n) {
            size_type new_nodes = (n + deque_buffer_size(BufSize, sizeof(T)) - 1) / deque_buffer_size(BufSize, sizeof(T));
            reserve_map_at_front(new_nodes);
            size_type i;
            try {
                for (i = 1; i <= new_nodes; ++i) {
                    *(m_start.m_node - i) = allocate_node();
                }
            }
            catch (const std::exception&) {
                for (size_type j = 1; j < i; ++j) {
                    deallocate_node(*(m_start.m_node - j));
                }
                throw;
            }
        }

        void new_elements_at_back(size_type n) {
            size_type new_nodes = (n + deque_buffer_size(BufSize, sizeof(T)) - 1) / deque_buffer_size(BufSize, sizeof(T));
            reserve_map_at_back(new_nodes);
            size_type i;
            try {
                for (i = 1; i <= new_nodes; ++i) {
                    *(m_finish.m_node + i) = allocate_node();
                }
            }
            catch (const std::exception&) {
                for (size_type j = 1; j < i; ++j) {
                    deallocate_node(*(m_finish.m_node + j));
                }
                throw;
            }
        }

        void reserve_map_at_front(size_type node_to_add = 1) {
            // If not enough space at front of the map.
            if (node_to_add > size_type(m_start.m_node - m_map)) {
                reallocate_map(node_to_add, true);
            }
        }

        void reserve_map_at_back(size_type node_to_add = 1) {
            // If not enough space at back of the map.
            if (node_to_add + 1 > m_map_size - (m_finish.m_node - m_map)) {
                reallocate_map(node_to_add, false);
            }
        }

        void push_back_aux(const value_type& value) {
            // Allocate a new node at the back.
            reserve_map_at_back();
            *(m_finish.m_node + 1) = allocate_node();
            try {
                // Construct the new element at the end of the old node.
                construct(m_finish.m_curr, value);
                // Switch to the new node.
                m_finish.set_node(m_finish.m_node + 1);
                m_finish.m_curr = m_finish.m_first;
            }
            catch (const std::exception&) {
                deallocate_node(*(m_finish.m_node + 1));
                throw;
            }
        }

        void push_back_aux() {
            // Allocate a new node at the back.
            reserve_map_at_back();
            *(m_finish.m_node + 1) = allocate_node();
            try {
                // Construct the new element at the end of the old node.
                construct(m_finish.m_curr);
                // Switch to the new node.
                m_finish.set_node(m_finish.m_node + 1);
                m_finish.m_curr = m_finish.m_first;
            }
            catch (const std::exception) {
                deallocate_node(*(m_finish.m_node + 1));
                throw;
            }
        }

        void push_front_aux(const value_type& value) {
            // Allocate a new node at the front.
            reserve_map_at_front();
            *(m_start.m_node - 1) = allocate_node();
            try {
                // Switch to the new node.
                m_start.set_node(m_start.m_node - 1);
                m_start.m_curr = m_start.m_last - 1;
                // Construct the new element at the beginning of the new node.
                construct(m_start.m_curr, value);
            }
            catch (const std::exception&) {
                ++m_start;
                deallocate_node(*(m_start.m_node - 1));
                throw;
            }
        }

        void push_front_aux() {
            // Allocate a new node at the front.
            reserve_map_at_front();
            *(m_start.m_node - 1) = allocate_node();
            try {
                // Switch to the new node.
                m_start.set_node(m_start.m_node - 1);
                m_start.m_curr = m_start.m_last - 1;
                // Construct the new element at the beginning of the new node.
                construct(m_start.m_curr);
            }
            catch (const std::exception) {
                ++m_start;
                deallocate_node(*(m_start.m_node - 1));
                throw;
            }
        }

        void pop_back_aux() {
            // Destroy the last element which is located at the beginning of the last node.
            deallocate_node(m_finish.m_first);
            // Switch to the the previous node.
            m_finish.set_node(m_finish.m_node - 1);
            // Update the current pointer.
            m_finish.m_curr = m_finish.m_last - 1;
            // Deconstruct the current element.
            destory(m_finish.m_curr);
        }

        void pop_front_aux() {
            // Deconstruct the current element.
            destory(m_start.m_curr);
            // Destroy the first element which is located at the end of the first node.
            deallocate_node(m_start.m_first);
            // Switch to the the next node.
            m_start.set_node(m_start.m_node + 1);
            // Update the current pointer.
            m_start.m_curr = m_start.m_first;
        }

        iterator insert_aux(iterator pos, const value_type& value) {
            const difference_type index = pos - m_start;
            // If in the first half.
            if (index < (difference_type)(size() / 2)) {
                push_front(front());
                // Map may be extended and changed memory position,
                // here we need to insert after `pos`.
                pos = m_start + index;
                // Copy [`m_start + 2`, `pos + 1`) to [`m_start + 1`, `pos`).
                copy(m_start + 2, pos + 1, m_start + 1);
            }
            // If in the second half.
            else {
                push_back(back());
                pos = m_start + index;
                // Copy [`pos`, `m_finish - 2`) to [`pos + 1`, `m_finish - 1`).
                copy_backward(pos, m_finish - 2, m_finish - 1);
            }
            *pos = value;
            return pos;
        }

        void insert_aux(iterator pos, size_type n, const value_type& value) {
            const difference_type index = pos - m_start;
            // If in the first half.
            if (index < (difference_type)(size() / 2)) {
                iterator new_start = reserve_element_at_front(n);
                iterator old_start = m_start;
                try {
                    if (index < difference_type(n)) {
                        // Copy [`old_start`, `pos`) to [`new_start`, `new_start + index`).
                        iterator new_index = uninitialized_copy(old_start, pos, new_start);
                        // Fill [`new_start + index`, `old_start`) with value.
                        uninitialized_fill(new_index, old_start, value);
                        // Fill [`old_start`, `pos`) with value.
                        fill(old_start, pos, value);
                    }
                    else {
                        // Copy [`old_start`, `old_start + n`) to [`new_start`, `new_start + n`).
                        iterator start_n = old_start + difference_type(n);
                        uninitialized_copy(old_start, start_n, new_start);
                        // Copy [`old_start + n`, `pos`) to [`new_start + n`, `new_start + index`).
                        copy(start_n, pos, old_start);
                        // Fill [`new_start + index`, `new_start + index + n`) with `value`.
                        fill(pos - difference_type(n), pos, value);
                    }
                    m_start = new_start;
                }
                catch (const std::exception&) {
                    destroy_nodes(new_start.m_node, old_start.m_node);
                    throw;
                }
            }
            // If in the second half.
            else {
                const difference_type element_after = m_finish - pos;
                iterator new_finish                 = reserve_element_at_back(n);
                iterator old_finish                 = m_finish;

                pos = old_finish - element_after;
                try {
                    if (element_after <= difference_type(n)) {
                        // Copy [`pos`, `old_finish`) to [`pos + n`, `new_finish`).
                        iterator pos_n = pos + difference_type(n);
                        uninitialized_copy(pos, m_finish, pos_n);
                        // Fill [`old_finish`, `pos + n`) with `value`.
                        uninitialized_fill(m_finish, pos_n, value);
                        // Fill [`pos`, `old_finish`) with `value`.
                        fill(pos, m_finish, value);
                    }
                    else {
                        // Copy [`old_finish - n`, `old_finish`) to [`old_finish`, `old_finish + n`).
                        iterator finish_n = old_finish - difference_type(n);
                        uninitialized_copy(finish_n, old_finish, old_finish);
                        // Copy [`pos`, `old_finish - n`) to [`pos + n`, `old_finish`).
                        copy_backward(pos, finish_n, old_finish);
                        // Fill [`pos`, `pos + n`) with `value`.
                        fill(pos, pos + difference_type(n), value);
                    }
                    m_finish = new_finish;
                }
                catch (const std::exception&) {
                    destroy_nodes(old_finish.m_node + 1, new_finish.m_node + 1);
                    throw;
                }
            }
        }

        void insert_aux(iterator pos, const value_type* first, const value_type* last, size_type n) {
            const difference_type index = pos - m_start;
            if (index < (difference_type)(size() / 2)) {
                iterator new_start = reserve_element_at_front(n);
                iterator old_start = m_start;
                try {
                    if (index < difference_type(n)) {
                        // Copy [`old_start`, `pos`) to [`new_start`, `new_start + index`).
                        const value_type* mid = first + (difference_type(n) - index);
                        uninitialized_copy(old_start, pos, new_start);
                        // Copy [`first`, `first + n - index`) to [`old_start`, `pos`).
                        uninitialized_copy(first, mid, pos - difference_type(n));
                        // Copy [`first + n - index`, `last`) to [`new_start + index`, `old_start`).
                        copy(mid, last, old_start);
                    }
                    else {
                        // Copy [`old_start`, `old_start + n`) to [`new_start`, `new_start + n`).
                        iterator start_n = old_start + difference_type(n);
                        uninitialized_copy(old_start, start_n, new_start);
                        // Copy [`old_start + n`, `pos`) to [`new_start + n`, `new_start + index`).
                        copy(start_n, pos, old_start);
                        // Copy [`first`, `last`) to [`pos - n`, `pos`).
                        copy(first, last, pos - difference_type(n));
                    }
                    m_start = new_start;
                }
                catch (const std::exception&) {
                    destroy_nodes(new_start.m_node, old_start.m_node);
                    throw;
                }
            }
            else {
                const difference_type element_after = m_finish - pos;
                iterator new_finish                 = reserve_element_at_back(n);
                iterator old_finish                 = m_finish;
                try {
                    if (element_after <= difference_type(n)) {
                        // Copy [`pos`, `old_finish`) to [`pos + n`, `new_finish`).
                        const value_type* mid = first + element_after;
                        uninitialized_copy(pos, old_finish, pos + difference_type(n));
                        // Copy [`mid`, `last`) to [`old_finish`, `pos + n`).
                        uninitialized_copy(mid, last, old_finish);
                        // Copy [`first`, `mid`) to [`pos`, `old_finish`).
                        copy(first, mid, pos);
                    }
                    else {
                        // Copy [`old_finish - n`, `old_finish`) to [`old_finish`, `old_finish + n`).
                        iterator finish_n = old_finish - difference_type(n);
                        uninitialized_copy(finish_n, old_finish, old_finish);
                        // Copy [`pos`, `old_finish - n`) to [`pos + n`, `old_finish`).
                        copy_backward(pos, finish_n, old_finish);
                        // Copy [`first`, `last`) to [`pos`, `pos + n`).
                        copy(first, last, pos);
                    }
                    m_finish = new_finish;
                }
                catch (const std::exception&) {
                    destroy_nodes(old_finish.m_node + 1, new_finish.m_node + 1);
                    throw;
                }
            }
        }

        void insert_aux(iterator pos, const_iterator first, const_iterator last, size_type n) {
            const difference_type index = pos - m_start;
            if (index < (difference_type)(size() / 2)) {
                iterator new_start = reserve_element_at_front(n);
                iterator old_start = m_start;
                try {
                    if (index < difference_type(n)) {
                        const value_type* mid = first + (difference_type(n) - index);
                        uninitialized_copy(old_start, pos, new_start);
                        uninitialized_copy(first, mid, pos - difference_type(n));
                        copy(mid, last, old_start);
                    }
                    else {
                        iterator start_n = old_start + difference_type(n);
                        uninitialized_copy(old_start, start_n, new_start);
                        copy(start_n, pos, old_start);
                        copy(first, last, pos - difference_type(n));
                    }
                    m_start = new_start;
                }
                catch (const std::exception& e) {
                    destroy_nodes(new_start.m_node, old_start.m_node);
                    throw;
                }
            }
            else {
                const difference_type element_after = m_finish - pos;
                iterator new_finish                 = reserve_element_at_back(n);
                iterator old_finish                 = m_finish;

                pos = old_finish - element_after;
                try {
                    if (element_after <= difference_type(n)) {
                        const value_type* mid = first + element_after;
                        uninitialized_copy(pos, old_finish, pos + difference_type(n));
                        uninitialized_copy(mid, last, old_finish);
                        copy(first, mid, pos);
                    }
                    else {
                        iterator finish_n = old_finish - difference_type(n);
                        uninitialized_copy(finish_n, old_finish, old_finish);
                        copy_backward(pos, finish_n, old_finish);
                        copy(first, last, pos);
                    }
                    m_finish = new_finish;
                }
                catch (const std::exception& e) {
                    destroy_nodes(old_finish.m_node + 1, new_finish.m_node + 1);
                    throw;
                }
            }
        }

    public:
        deque() { initialize_map(0); }

        explicit deque(size_t n, const value_type& value = value_type()) {
            initialize_map(n);
            fill_initialize(value);
        }

        deque(const deque& other) {
            initialize_map(other.size());
            TinySTL::uninitialized_copy(other.begin(), other.end(), begin());
        }

        deque(const value_type* first, const value_type* last) {
            initialize_map(last - first);
            TinySTL::uninitialized_copy(first, last, begin());
        }

        deque(const_iterator first, const_iterator last) {
            initialize_map(last - first);
            TinySTL::uninitialized_copy(first, last, begin());
        }

        deque& operator=(const deque& other) {
            if (this != &other) {
                const size_type len = size();
                if (len >= other.size()) {
                    iterator new_finish = copy(other.begin(), other.end(), begin());
                    erase(new_finish, m_finish);
                }
                else {
                    const_iterator mid = other.begin() + difference_type(len);
                    copy(other.begin(), mid, begin());
                    insert(m_finish, mid, other.end());
                }
            }
            return *this;
        }

        ~deque() {
            clear();
            deallocate_map(m_map, m_map_size);
        }

        iterator begin() { return m_start; }
        iterator end() { return m_finish; }
        const_iterator begin() const { return m_start; }
        const_iterator end() const { return m_finish; }
        const_iterator cbegin() const { return m_start; }
        const_iterator cend() const { return m_finish; }

        reference front() { return *m_start; }
        reference back() { return *(m_finish - 1); }
        const_reference front() const { return *m_start; }
        const_reference back() const { return *(m_finish - 1); }
        reference operator[](size_t idx) { return m_start[difference_type(idx)]; }
        const_reference operator[](size_t idx) const { return m_start[difference_type(idx)]; }

        size_type size() const { return m_finish - m_start; }
        bool empty() const { return m_start == m_finish; }

        void push_back(const value_type& value) {
            // If not at the last - 1 position of current buffer.
            if (m_finish.m_curr != m_finish.m_last - 1) {
                construct(m_finish.m_curr, value);
                ++m_finish.m_curr;
            }
            else {
                push_back_aux(value);
            }
        }

        void push_back() {
            // If not at the last - 1 position of current buffer.
            if (m_finish.m_curr != m_finish.m_last - 1) {
                construct(m_finish.m_curr);
                ++m_finish.m_curr;
            }
            else {
                push_back_aux();
            }
        }

        void push_front(const value_type& value) {
            // If not at the first position of current buffer.
            if (m_start.m_curr != m_start.m_first) {
                construct(m_start.m_curr - 1, value);
                --m_start.m_curr;
            }
            else {
                push_front_aux(value);
            }
        }

        void push_front() {
            // If not at the first position of current buffer.
            if (m_start.m_curr != m_start.m_first) {
                construct(m_start.m_curr - 1);
                --m_start.m_curr;
            }
            else {
                push_front_aux();
            }
        }

        void pop_back() {
            // If not at the first position of current buffer.
            if (m_finish.m_curr != m_finish.m_first) {
                --m_finish.m_curr;
                destory(m_finish.m_curr);
            }
            else {
                pop_back_aux();
            }
        }

        void pop_front() {
            // If not at the last - 1 position of current buffer.
            if (m_start.m_curr != m_start.m_last - 1) {
                destory(m_start.m_curr);
                ++m_start.m_curr;
            }
            else {
                pop_front_aux();
            }
        }

        // Insert at the front of the `pos`.
        iterator insert(iterator pos, const value_type& value = value_type()) {
            // Insert at front.
            if (pos.m_curr == m_start.m_curr) {
                push_front(value);
                return m_start;
            }
            // Insert at back.
            else if (pos.m_curr == m_finish.m_curr) {
                push_back(value);
                return m_finish - 1;
            }
            // Insert in the middle.
            else {
                return insert_aux(pos, value);
            }
        }

        void insert(iterator pos, size_type n, const value_type& value = value_type()) {
            // If insert at the front, move the m_start.
            if (pos.m_curr == m_start.m_curr) {
                iterator new_start = reserve_element_at_front(n);
                uninitialized_fill(new_start, m_start, value);
                m_start = new_start;
            }
            // If insert at the back, move the m_finish.
            else if (pos.m_curr == m_finish.m_curr) {
                iterator new_finish = reserve_element_at_back(n);
                uninitialized_fill(m_finish, new_finish, value);
                m_finish = new_finish;
            }
            else {
                insert_aux(pos, n, value);
            }
        }

        void insert(iterator pos, const value_type* first, const value_type* last) {
            size_type n = last - first;
            // If insert at the front, move the m_start.
            if (pos.m_curr == m_start.m_curr) {
                iterator new_start = reserve_element_at_front(n);
                try {
                    uninitialized_copy(first, last, new_start);
                    m_start = new_start;
                }
                catch (const std::exception&) {
                    destroy_nodes(new_start.m_node, m_start.m_node);
                    throw;
                }
            }
            // If insert at the back, move the m_finish.
            else if (pos.m_curr == m_finish.m_curr) {
                iterator new_finish = reserve_element_at_back(n);
                try {
                    uninitialized_copy(first, last, m_finish);
                    m_finish = new_finish;
                }
                catch (const std::exception&) {
                    destroy_nodes(m_finish.m_node + 1, new_finish.m_node + 1);
                    throw;
                }
            }
            else {
                insert_aux(pos, first, last, n);
            }
        }

        void insert(iterator pos, const_iterator first, const_iterator last) {
            size_type n = last - first;
            if (pos.m_curr == m_start.m_curr) {
                iterator new_start = reserve_element_at_front(n);
                try {
                    uninitialized_copy(first, last, new_start);
                    m_start = new_start;
                }
                catch (const std::exception& e) {
                    destroy_nodes(new_start.m_node, m_start.m_node);
                    throw;
                }
            }
            else if (pos.m_curr == m_finish.m_curr) {
                iterator new_finish = reserve_element_at_back(n);
                try {
                    uninitialized_copy(first, last, m_finish);
                    m_finish = new_finish;
                }
                catch (const std::exception& e) {
                    destroy_nodes(m_finish.m_node + 1, new_finish.m_node + 1);
                    throw;
                }
            }
            else {
                insert_aux(pos, first, last, n);
            }
        }

        iterator erase(iterator pos) {
            iterator next = pos;
            ++next;
            difference_type index = pos - m_start;
            // At the first half.
            if (index < (difference_type)(size() / 2)) {
                copy_backward(m_start, pos, next);
                pop_front();
            }
            // At the second half.
            else {
                copy(next, m_finish, pos);
                pop_back();
            }
            return m_start + index;
        }

        iterator erase(iterator first, iterator last) {
            // If erase all.
            if (first == m_start && last == m_finish) {
                clear();
                return m_finish;
            }

            difference_type n     = last - first;
            difference_type index = first - m_start;
            if (index < (difference_type)((size() - n) / 2)) {
                // Move [`m_start`, `first`) to [`m_start + n`, `last`).
                copy_backward(m_start, first, last);
                iterator new_start = m_start + n;
                destory(m_start, new_start);
                destroy_nodes(m_start.m_node, new_start.m_node);
                m_start = new_start;
            }
            else {
                // Move [`last`, `m_finish`) to [`first`, `m_finish - n`).
                copy(last, m_finish, first);
                iterator new_finish = m_finish - n;
                destory(new_finish, m_finish);
                destroy_nodes(new_finish.m_node + 1, m_finish.m_node + 1);
                m_finish = new_finish;
            }
            return m_start + index;
        }

        void resize(size_type new_size, const value_type& value = value_type()) {
            const size_t len = size();
            if (new_size < len) {
                erase(m_start + new_size, m_finish);
            }
            else {
                insert(m_finish, new_size - len, value);
            }
        }

        void clear() {
            // Destroy the elements in the middle.
            for (map_pointer curr_node = m_start.m_node + 1; curr_node < m_finish.m_node; ++curr_node) {
                destory(*curr_node, *curr_node + deque_buffer_size(BufSize, sizeof(T)));
                deallocate_node(*curr_node);
            }

            // Destroy the elements in the first and last node.
            if (m_start.m_node != m_finish.m_node) {
                destory(m_start.m_curr, m_start.m_last);
                destory(m_finish.m_first, m_finish.m_curr);
                deallocate_node(m_finish.m_first);
            }
            else {
                destory(m_start.m_curr, m_finish.m_curr);
            }

            m_finish = m_start;
        }

        void swap(deque& other) {
            TinySTL::swap(m_start, other.m_start);
            TinySTL::swap(m_finish, other.m_finish);
            TinySTL::swap(m_map, other.m_map);
            TinySTL::swap(m_map_size, other.m_map_size);
        }

        bool operator==(const deque& other) const {
            return (size() == other.size() && equal(begin(), end(), other.begin()));
        }

        bool operator!=(const deque& other) const {
            return !operator==(other);
        }

        bool operator<=(const deque& other) const {
            return lexicographical_compare(begin(), end(), other.begin(), other.end());
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_DEQUE_HPP_
