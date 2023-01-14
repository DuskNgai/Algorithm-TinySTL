#ifndef _TINYSTL_LIST_HPP_
#define _TINYSTL_LIST_HPP_

#include <stl_algorithm.hpp>
#include <stl_allocator.hpp>
#include <stl_iterator.hpp>
#include <stl_type_traits.hpp>
#include <stl_uninitialized.hpp>

namespace TinySTL {

    template <typename T>
    struct list_node {
        // clang-format off
        list_node<T>* prev;
        list_node<T>* next;
        T             data;
        // clang-format on
    };

    template <typename T, typename Ref, typename Ptr>
    struct list_iterator {
        // clang-format off
        using iterator       = list_iterator<T, T&, T*>;
        using const_iterator = list_iterator<T, const T&, const T*>;
        using self           = list_iterator<T, Ref, Ptr>;

        using iterator_category = bidirectional_iterator_tag;
        using value_type        = T;
        using pointer           = Ptr;
        using reference         = Ref;
        using size_type         = size_t;
        using difference_type   = ptrdiff_t;

        using node = list_node<T> ;
        // clang-format on

    public:
        node* m_inner;

        list_iterator(node* n = nullptr)
            : m_inner(n) {}
        list_iterator(const iterator& other)
            : m_inner(other.m_inner) {}

        bool operator==(const self& other) const { return m_inner == other.m_inner; }
        bool operator!=(const self& other) const { return m_inner != other.m_inner; }
        reference operator*() const { return m_inner->data; }
        pointer operator->() const { return &(operator*()); }

        self& operator++() {
            m_inner = m_inner->next;
            return *this;
        }
        self operator++(int) {
            node* temp = m_inner;
            ++*this;
            return temp;
        }
        self& operator--() {
            m_inner = m_inner->prev;
            return *this;
        }
        self operator--(int) {
            node* temp = m_inner;
            --*this;
            return temp;
        }
    };

    template <typename T, typename Alloc = alloc>
    class list {
    public:
        // clang-format off
        using value_type      = T;
        using pointer         = T*;
        using reference       = T&;
        using size_type       = size_t ;
        using difference_type = ptrdiff_t;
        using const_pointer   = const T*;
        using const_reference = const T&;
        using node            = list_node<T>;

        using list_allocator = simple_alloc<node, Alloc>;

        using iterator       = list_iterator<T, T&, T*>;
        using const_iterator = list_iterator<T, const T&, const T*>;
        // clang-format on

    protected:
        node* m_sentinel;

    private:
        node* get_node() { return list_allocator::allocate((size_type)1); }
        void put_node(node* n) { list_allocator::deallocate(n); }
        node* create_node(const T& value) {
            node* n = get_node();
            try {
                construct(&(n->data), value);
            }
            catch (const std::exception&) {
                put_node(n);
                throw;
            }
            return n;
        }

        void init() {
            m_sentinel = get_node();
            m_sentinel->next = m_sentinel;
            m_sentinel->prev = m_sentinel;
        }

    public:
        list() { init(); }

        explicit list(size_type n) {
            init();
            insert(begin(), n, value_type());
        }

        list(size_type n, const T& value) {
            init();
            insert(begin(), n, value);
        }

        list(const T* first, const T* last) {
            init();
            insert(begin(), first, last);
        }

        list(const_iterator first, const_iterator last) {
            init();
            insert(begin(), first, last);
        }

        list(const list& other) {
            init();
            insert(begin(), other.begin(), other.end());
        }

        list& operator=(const list& other) {
            if (this != &other) {
                // Copy the same length part.
                iterator first1 = begin();
                iterator last1 = end();
                const_iterator first2 = other.begin();
                const_iterator last2 = other.end();
                while (first1 != last1 && first2 != last2) {
                    *first1++ = *first2++;
                }
                // If the other list is shorter, erase the rest.
                if (first2 == last2) {
                    erase(first1, last1);
                }
                // If the other list is longer, insert the rest.
                else {
                    insert(last1, first2, last2);
                }
            }
            return *this;
        }

        ~list() {
            clear();
            put_node(m_sentinel);
        }

        iterator begin() { return m_sentinel->next; }
        iterator end() { return m_sentinel; }
        const_iterator begin() const { return m_sentinel->next; }
        const_iterator end() const { return m_sentinel; }
        const_iterator cbegin() const { return m_sentinel->next; }
        const_iterator cend() const { return m_sentinel; }

        size_type size() const { return distance(begin(), end()); }
        bool empty() { return m_sentinel->next == m_sentinel; }

        reference front() { return *begin(); }
        reference back() { return *(--end()); }
        const_reference front() const { return *begin(); }
        const_reference back() const { return *(--end()); }

        bool operator==(const list& other) const {
            node* e1 = m_sentinel;
            node* e2 = other.m_sentinel;
            node* b1 = m_sentinel->next;
            node* b2 = other.m_sentinel->next;
            // Same data.
            for (; b1 != e2 && b2 != e2; b1 = b1->next, b2 = b2->next) {
                if (b1->data != b2->data)
                    return false;
            }
            // Same size.
            return b1 == e2 && b2 == e2;
        }

        bool operator!=(const list& other) const {
            return !operator==(other);
        }

        bool operator<(const list& other) const {
            return TinySTL::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }

        iterator insert(iterator pos, const T& value = value_type()) {
            node* temp = create_node(value);
            temp->next = pos.m_inner;
            temp->prev = pos.m_inner->prev;
            pos.m_inner->prev->next = temp;
            pos.m_inner->prev = temp;
            return iterator(temp);
        }

        void insert(iterator pos, const T* first, const T* last) {
            for (; first != last; ++first) {
                insert(pos, *first);
            }
        }

        void insert(iterator pos, const_iterator first, const_iterator last) {
            for (; first != last; ++first) {
                insert(pos, *first);
            }
        }

        void insert(iterator pos, size_type n, const T& value) {
            for (; n > 0; --n) {
                insert(pos, value);
            }
        }

        void push_front(const T& value = value_type()) { insert(begin(), value); }
        void push_back(const T& value = value_type()) { insert(end(), value); }

        iterator erase(iterator pos) {
            node* next_node = pos.m_inner->next;
            node* prev_node = pos.m_inner->prev;
            prev_node->next = next_node;
            next_node->prev = prev_node;
            destory(&(pos.m_inner->data));
            put_node(pos.m_inner);
            return iterator(next_node);
        }
        iterator erase(iterator first, iterator last) {
            while (first != last) {
                erase(first++);
            }
            return last;
        }

        void pop_front() { erase(begin()); }
        void pop_back() {
            iterator temp = end();
            erase(--temp);
        }

        void resize(size_type new_size, const T& value = value_type()) {
            iterator it = begin();
            size_type len = 0;
            for (; it != end() && len < new_size; ++it, ++len)
                ;
            // If too long, erase the rest.
            if (len == new_size) {
                erase(it, end());
            }
            // If too short, insert the rest.
            else {
                insert(end(), new_size - len, value);
            }
        }

        void clear() {
            node* curr = m_sentinel->next;
            while (curr != m_sentinel) {
                node* temp = curr;
                curr = curr->next;
                destory(&(temp->data));
                put_node(temp);
            }
            m_sentinel->next = m_sentinel;
            m_sentinel->prev = m_sentinel;
        }

        void swap(list& other) {
            std::swap(m_sentinel, other.m_sentinel);
        }

        void remove(const T& value) {
            iterator first = begin();
            iterator last = end();
            while (first != last) {
                iterator next = first;
                ++next;
                if (*first == value) {
                    erase(first);
                }
                first = next;
            }
        }

        void unique() {
            iterator first = begin();
            iterator last = end();
            if (first == last) {
                return;
            }
            iterator next = first;
            while (++next != last) {
                if (*first == *next) {
                    erase(next);
                }
                else {
                    first = next;
                }
                next = first;
            }
        }

    protected:
        // Move [first, last) to the position end with `pos`.
        void transfer(iterator pos, iterator first, iterator last) {
            if (pos != last) {
                // Next connection of `pos`.
                last.m_inner->prev->next = pos.m_inner;
                first.m_inner->prev->next = last.m_inner;
                pos.m_inner->prev->next = first.m_inner;

                // Prev connection of `pos`.
                node* temp = pos.m_inner->prev;
                pos.m_inner->prev = last.m_inner->prev;
                last.m_inner->prev = first.m_inner->prev;
                first.m_inner->prev = temp;
            }
        }

    public:
        void splice(iterator pos, list& other) {
            if (!other.empty()) {
                transfer(pos, other.begin(), other.end());
            }
        }

        void splice(iterator pos, list&, iterator i) {
            iterator j = i;
            ++j;
            if (pos == i || pos == j) {
                return;
            }
            transfer(pos, i, j);
        }

        void splice(iterator pos, list&, iterator first, iterator last) {
            if (first != last) {
                transfer(pos, first, last);
            }
        }

        // Merge two sorted list in ascending order.
        void merge(list& other) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = other.begin();
            iterator last2 = other.end();

            while (first1 != last1 && first2 != last2) {
                // It may only overload <.
                if (*first2 < *first1) {
                    iterator next = first2;
                    transfer(first1, first2, ++next);
                    // No ++first2 here, because first2 is after first1 now,
                    // which means first2 is on this list.
                    first2 = next;
                }
                else {
                    // If the same, list1 first.
                    ++first1;
                }
            }
            if (first2 != last2) {
                transfer(last1, first2, last2);
            }
        }

        template <typename Compare>
        void merge(list& other, Compare comp) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = other.begin();
            iterator last2 = other.end();

            while (first1 != last1 && first2 != last2) {
                if (comp(*first2, *first1)) {
                    iterator next = first2;
                    transfer(first1, first2, ++next);
                    first2 = next;
                }
                else {
                    ++first1;
                }
            }
            if (first2 != last2) {
                transfer(last1, first2, last2);
            }
        }

        void reverse() {
            // Do nothing if size() is 0 or 1.
            if (m_sentinel->next != m_sentinel && m_sentinel->next->next != m_sentinel) {
                iterator first = begin();
                ++first;
                while (first != end()) {
                    iterator old = first;
                    ++first;
                    // Move to begin() one by one.
                    transfer(begin(), old, first);
                }
            }
        }

        // Merge sort.
        void sort() {
            // Do nothing if size() is 0 or 1.
            if (m_sentinel->next != m_sentinel && m_sentinel->next->next != m_sentinel) {
                list carry;
                // Upper limit 2 ^ 64.
                list counter[64];

                int fill = 0;
                while (!empty()) {
                    // Put the begin() to carry's begin() one by one.
                    carry.splice(carry.begin(), *this, begin());
                    int i = 0;
                    while (i < fill && !counter[i].empty()) {
                        // Here we sort it.
                        counter[i].merge(carry);
                        carry.swap(counter[i++]);
                    }
                    carry.swap(counter[i]);
                    if (i == fill) {
                        ++fill;
                    }
                }
                for (int i = 1; i < fill; ++i) {
                    counter[i].merge(counter[i - 1]);
                }
                swap(counter[fill - 1]);
            }
        }

        template <typename Compare>
        void sort(Compare comp) {
            // Do nothing if size() is 0 or 1.
            if (m_sentinel->next != m_sentinel && m_sentinel->next->next != m_sentinel) {
                list carry;
                // Upper limit 2 ^ 64.
                list counter[64];

                int fill = 0;
                while (!empty()) {
                    // Put the begin() to carry's begin() one by one.
                    carry.splice(carry.begin(), *this, begin());
                    int i = 0;
                    while (i < fill && !counter[i].empty()) {
                        // Here we sort it.
                        counter[i].merge(carry, comp);
                        carry.swap(counter[i++]);
                    }
                    carry.swap(counter[i]);
                    if (i == fill) {
                        ++fill;
                    }
                }
                for (int i = 1; i < fill; ++i) {
                    counter[i].merge(counter[i - 1]);
                }
                swap(counter[fill - 1]);
            }
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_LIST_HPP_
