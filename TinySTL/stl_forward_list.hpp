#ifndef _TINYSTL_FORWARD_LIST_HPP_
#define _TINYSTL_FORWARD_LIST_HPP_

#include <stl_allocator.hpp>
#include <stl_iterator.hpp>
#include <stl_uninitialized.hpp>

namespace TinySTL {

    struct forward_list_node_base {
        forward_list_node_base* next;
    };

    inline forward_list_node_base* forward_list_make_link(forward_list_node_base* prev, forward_list_node_base* curr) {
        curr->next = prev->next;
        prev->next = curr;
        return curr;
    }

    inline forward_list_node_base* forward_list_previous(forward_list_node_base* head, const forward_list_node_base* curr) {
        while (head && head->next != curr) {
            head = head->next;
        }
        return head;
    }

    inline const forward_list_node_base* forward_list_previous(const forward_list_node_base* head, const forward_list_node_base* curr) {
        while (head && head->next != curr) {
            head = head->next;
        }
        return head;
    }

    inline void forward_list_splice_after(forward_list_node_base* pos, forward_list_node_base* before_first, forward_list_node_base* before_last) {
        if (pos != before_first && pos != before_last) {
            forward_list_node_base* first     = before_first->next;
            forward_list_node_base* after_pos = pos->next;

            before_first->next = before_last->next;
            pos->next          = first;
            before_last->next  = after_pos;
        }
    }

    inline forward_list_node_base* forward_list_reverse(forward_list_node_base* head) {
        forward_list_node_base* curr = head;
        head                         = head->next;
        curr->next                   = nullptr;

        // Flip one by one.
        while (head) {
            forward_list_node_base* next = head->next;

            head->next = curr;
            curr       = head;
            head       = next;
        }
        return curr;
    }

    inline size_t forward_list_size(const forward_list_node_base* head) {
        size_t result = 0;
        for (; head != nullptr; head = head->next) {
            ++result;
        }
        return result;
    }

    template <typename T>
    struct forward_list_node : public forward_list_node_base {
        T data;
    };

    struct forward_list_iterator_base {
        using size_type         = size_t;
        using difference_type   = ptrdiff_t;
        using iterator_category = forward_iterator_tag;

    public:
        forward_list_node_base* m_inner;

        forward_list_iterator_base(forward_list_node_base* inner)
            : m_inner(inner) {}

        void increment() { m_inner = m_inner->next; }

        friend bool operator==(const forward_list_iterator_base& lhs, const forward_list_iterator_base& rhs) noexcept { return lhs.m_inner == rhs.m_inner; }
        friend bool operator!=(const forward_list_iterator_base& lhs, const forward_list_iterator_base& rhs) noexcept { return !(lhs == rhs); }
    };

    template <typename T, typename Ref, typename Ptr>
    struct forward_list_iterator : public forward_list_iterator_base {
        using iterator       = forward_list_iterator<T, T&, T*>;
        using const_iterator = forward_list_iterator<T, const T&, const T*>;
        using self           = forward_list_iterator<T, Ref, Ptr>;

        using value_type = T;
        using pointer    = Ptr;
        using reference  = Ref;
        using node       = forward_list_node<T>;

        forward_list_iterator(node* inner = nullptr)
            : forward_list_iterator_base(inner) {}

        forward_list_iterator(const iterator& other)
            : forward_list_iterator_base(other.m_inner) {}

        reference operator*() const { return ((node*)m_inner)->data; }

        pointer operator->() const { return &(operator*()); }

        self& operator++() {
            increment();
            return *this;
        }

        self operator++(int) {
            self temp = *this;
            increment();
            return temp;
        }
    };

    constexpr ptrdiff_t* distance_type(const forward_list_iterator_base&) {
        return nullptr;
    }

    template <typename T, typename Ref, typename Ptr>
    constexpr T* value_type(const forward_list_iterator<T, Ref, Ptr>&) {
        return nullptr;
    }

    constexpr forward_iterator_tag iterator_category(const forward_list_iterator_base&) {
        return forward_iterator_tag();
    }

    template <typename T, typename Alloc = alloc>
    class forward_list {
    public:
        using value_type      = T;
        using pointer         = T*;
        using reference       = T&;
        using const_pointer   = const T*;
        using const_reference = const T&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;

        using iterator       = forward_list_iterator<T, T&, T*>;
        using const_iterator = forward_list_iterator<T, const T&, const T*>;

        using forward_list_allocator = simple_alloc<forward_list_node<T>, Alloc>;

    private:
        using list_node      = forward_list_node<T>;
        using list_node_base = forward_list_node_base;
        using iterator_base  = forward_list_iterator_base;

        list_node* get_node() { return forward_list_allocator::allocate(); }

        void put_node(list_node* node) { forward_list_allocator::deallocate(node); }

        list_node* create_node(const value_type& value) {
            list_node* node = get_node();
            try {
                construct(&node->data, value);
                node->next = nullptr;
            }
            catch (const std::exception&) {
                put_node(node);
                throw;
            }
            return node;
        }

        list_node* create_node() {
            list_node* node = get_node();
            try {
                construct(&node->data);
                node->next = nullptr;
            }
            catch (const std::exception&) {
                put_node(node);
                throw;
            }
        }

        void destory_node(list_node* node) {
            destory(&node->data);
            put_node(node);
        }

    private:
        list_node_base m_head;

    public:
        forward_list() { m_head.next = nullptr; }

        explicit forward_list(size_type n, const value_type& value = value_type()) {
            m_head.next = nullptr;
            insert_after_fill(&m_head, n, value);
        }

        forward_list(const value_type* first, const value_type* last) {
            m_head.next = nullptr;
            insert_after_range(&m_head, first, last);
        }

        forward_list(const_iterator first, const_iterator last) {
            m_head.next = nullptr;
            insert_after_range(&m_head, first, last);
        }

        forward_list(const forward_list& other) {
            m_head.next = nullptr;
            insert_after_range(&m_head, other.begin(), other.end());
        }

        forward_list& operator=(const forward_list& other) {
            if (this != &other) {
                list_node_base* last        = &m_head;
                list_node* this_curr        = (list_node*)m_head.next;
                const list_node* other_curr = (const list_node*)other.m_head.next;
                while (this_curr && other_curr) {
                    this_curr->data = other_curr->data;
                    last            = this_curr;
                    this_curr       = (list_node*)this_curr->next;
                    other_curr      = (const list_node*)other_curr->next;
                }
                // Other is shorter.
                if (other_curr == nullptr) {
                    erase_after_aux(last, nullptr);
                }
                else {
                    insert_after_range(last, const_iterator(other_curr), const_iterator(nullptr));
                }
            }
            return *this;
        }

        ~forward_list() { clear(); }

    private:
        list_node* insert_after_aux(list_node_base* pos, const value_type& value) {
            return (list_node*)forward_list_make_link(pos, create_node(value));
        }

        list_node* insert_after_aux(list_node_base* pos) {
            return (list_node*)forward_list_make_link(pos, create_node());
        }

        void insert_after_fill(list_node_base* pos, size_type n, const value_type& value) {
            for (size_type i = 0; i < n; ++i) {
                pos = insert_after_aux(pos, value);
            }
        }

        void insert_after_range(list_node_base* pos, const value_type* first, const value_type* last) {
            for (; first != last; ++first) {
                pos = insert_after_aux(pos, *first);
            }
        }

        void insert_after_range(list_node_base* pos, const_iterator first, const_iterator last) {
            for (; first != last; ++first) {
                pos = insert_after_aux(pos, *first);
            }
        }

        list_node_base* erase_after_aux(list_node_base* pos) {
            list_node* next           = (list_node*)pos->next;
            list_node_base* next_next = next->next;
            pos->next                 = next_next;
            destory_node(next);
            return next_next;
        }

        list_node_base* erase_after_aux(list_node_base* before_first, list_node_base* last) {
            while (before_first->next != last) {
                erase_after_aux(before_first);
            }
            return last;
        }

    public:
        iterator begin() { return iterator((list_node*)m_head.next); }
        iterator end() { return iterator(nullptr); }
        const_iterator begin() const { return const_iterator((list_node*)m_head.next); }
        const_iterator end() const { return const_iterator(nullptr); }
        const_iterator cbegin() const { return const_iterator((list_node*)m_head.next); }
        const_iterator cend() const { return const_iterator(nullptr); }

        size_type size() const { return forward_list_size(m_head.next); }
        bool empty() const { return m_head.next == nullptr; }

        reference front() { return ((list_node*)m_head.next)->data; }
        const_reference front() const { return ((list_node*)m_head.next)->data; }

        iterator previous(const_iterator pos) {
            return iterator((list_node*)forward_list_previous(&m_head, pos.m_inner));
        }
        const_iterator previous(const_iterator pos) const {
            return const_iterator((list_node*)forward_list_previous(&m_head, pos.m_inner));
        }

        friend void swap(forward_list& lhs, forward_list& rhs) {
            using TinySTL::swap;
            swap(lhs.m_head.next, rhs.m_head.next);
        }

        void push_front(const value_type& value) {
            forward_list_make_link(&m_head, create_node(value));
        }

        void push_front() {
            forward_list_make_link(&m_head, create_node());
        }

        void pop_front() {
            list_node* node = (list_node*)m_head.next;
            m_head.next     = node->next;
            destroy_nodes(node);
        }

        iterator insert_after(const_iterator pos, const value_type& value = value_type()) {
            return iterator(insert_after_aux(pos.m_inner, value));
        }

        void insert_after(iterator pos, size_type n, const value_type& value) {
            insert_after_fill(pos.m_inner, n, value);
        }

        void insert_after(iterator pos, const value_type* first, const value_type* last) {
            insert_after_range(pos.m_inner, first, last);
        }

        void insert_after(iterator pos, const_iterator first, const_iterator last) {
            insert_after_range(pos.m_inner, first, last);
        }

        iterator insert(iterator pos, const value_type& value = value_type()) {
            return iterator(insert_after_aux(forward_list_previous(&m_head, pos.m_inner), value));
        }

        void insert(iterator pos, size_type n, const value_type& value) {
            insert_after_fill(forward_list_previous(&m_head, pos.m_inner), n, value);
        }

        void insert(iterator pos, const value_type* first, const value_type* last) {
            insert_after_range(forward_list_previous(&m_head, pos.m_inner), first, last);
        }

        void insert(iterator pos, const_iterator first, const_iterator last) {
            insert_after_range(forward_list_previous(&m_head, pos.m_inner), first, last);
        }

        iterator erase_after(iterator pos) {
            return iterator((list_node*)erase_after_aux(pos.m_inner));
        }

        iterator erase_after(iterator before_first, iterator last) {
            return iterator((list_node*)erase_after_aux(before_first.m_inner, last.m_inner));
        }

        iterator erase(iterator pos) {
            return iterator((list_node*)erase_after_aux(forward_list_previous(&m_head, pos.m_inner)));
        }

        iterator erase(iterator first, iterator last) {
            return iterator((list_node*)erase_after_aux(forward_list_previous(&m_head, first.m_inner), last.m_inner));
        }

        void splice_after(iterator pos, iterator before_first, iterator before_last) {
            if (before_first != before_last) {
                forward_list_splice_after(pos.m_inner, before_first.m_inner, before_last.m_inner);
            }
        }

        void splice_after(iterator pos, iterator prev) {
            forward_list_splice_after(pos.m_inner, prev.m_inner, prev.m_inner->next);
        }

        void splice(iterator pos, forward_list& other) {
            if (other.m_head.next) {
                forward_list_splice_after(
                    forward_list_previous(&m_head, pos.m_inner),
                    &other.m_head,
                    forward_list_previous(&other.m_head, nullptr));
            }
        }

        void splice(iterator pos, forward_list& other, iterator prev) {
            forward_list_splice_after(
                forward_list_previous(&m_head, pos.m_inner),
                forward_list_previous(&other.m_head, prev.m_inner),
                prev.m_inner);
        }

        void splice(iterator pos, iterator first, iterator last) {
            if (first != last) {
                forward_list_splice_after(
                    forward_list_previous(&m_head, pos.m_inner),
                    forward_list_previous(&m_head, first.m_inner),
                    forward_list_previous(first.m_inner, last.m_inner)
            }
        }

        void reverse() {
            if (m_head.next) {
                m_head.next = forward_list_reverse(m_head.next);
            }
        }

        void resize(size_type new_size, const T& value = value_type()) {
            list_node_base* curr = &m_head;
            while (curr->next && new_size > 0) {
                curr = curr->next;
                --new_size;
            }
            if (curr->next != nullptr) {
                erase_after_aux(curr, nullptr);
            }
            else {
                insert_after_fill(curr, new_size, value);
            }
        }

        void clear() { erase_after_aux(&m_head, 0); }

        void remove(const T& value) {
            list_node_base* curr = &m_head;
            while (curr && curr->next) {
                if (((list_node*)curr->next)->data == value) {
                    erase_after_aux(curr);
                }
                else {
                    curr = curr->next;
                }
            }
        }

        void unique() {
            list_node_base* curr = m_head.next;
            if (curr) {
                while (curr->next) {
                    if (((list_node*)curr)->data == ((list_node*)curr->next)->data) {
                        erase_after_aux(curr);
                    }
                    else {
                        curr = curr->next;
                    }
                }
            }
        }

        // Other can be the same forward list.
        void merge(forward_list& other) {
            list_node_base* curr = &m_head;
            while (curr->next && other.m_head.next) {
                if (((list_node*)curr->next)->data < ((list_node*)other.m_head.next)->data) {
                    forward_list_splice_after(curr, &other.m_head, other.m_head.next);
                }
                else {
                    curr = curr->next;
                }
            }
            // Other list is longer.
            if (other.m_head.next) {
                curr->next        = other.m_head.next;
                other.m_head.next = nullptr;
            }
        }

        // Similar to `list::sort`.
        void sort() {
            // Length <= 1 is already sorted.
            if (m_head.next && m_head.next->next) {
                forward_list carry;
                forward_list counter[64];
                int fill = 0;
                while (!empty()) {
                    forward_list_splice_after(&carry.m_head, &m_head, m_head.next);
                    int i = 0;
                    while (i < fill && !counter[i].empty()) {
                        counter[i].merge(carry);
                        carry.swap(counter[i++]);
                        ++i;
                    }
                    carry.swap(counter[i]);
                    if (i == fill) {
                        ++fill;
                    }
                }

                for (int i = 1; i < fill; ++i) {
                    counter[i].merge(counter[i - 1]);
                }
                this->swap(counter[fill - 1]);
            }
        }

        friend bool operator==(const forward_list& lhs, const forward_list& rhs) {
            list_node* curr1 = (list_node*)lhs.m_head.next;
            list_node* curr2 = (list_node*)rhs.m_head.next;
            while (curr1 && curr2 && curr1->data == curr2->data) {
                curr1 = (list_node*)curr1->next;
                curr2 = (list_node*)curr2->next;
            }
            return curr1 == nullptr && curr2 == nullptr;
        }

        friend bool operator!=(const forward_list& lhs, const forward_list& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const forward_list& lhs, const forward_list& rhs) {
            return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

        friend bool operator<=(const forward_list& lhs, const forward_list& rhs) {
            return !(rhs < lhs);
        }

        friend bool operator>(const forward_list& lhs, const forward_list& rhs) {
            return rhs < lhs;
        }

        friend bool operator>=(const forward_list& lhs, const forward_list& rhs) {
            return !(lhs < rhs);
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_FORWARD_LIST_HPP_
