#ifndef _ALGORITHM_AVL_TREE_HPP_
#define _ALGORITHM_AVL_TREE_HPP_

#include <cstdlib>
#include <iostream>

namespace Algorithm {

    using avltree_height_type                                   = std::size_t;
    static constexpr avltree_height_type avltree_balance_factor = 1;

    struct avltree_node_base {
        using height_type = avltree_height_type;
        using base_ptr    = avltree_node_base*;

        height_type height;
        base_ptr left;
        base_ptr right;
        base_ptr parent;

    public:
        static base_ptr s_minimum(base_ptr node) {
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        static base_ptr s_maximum(base_ptr node) {
            while (node->right != nullptr) {
                node = node->right;
            }
            return node;
        }
    };

    template <typename T>
    struct avltree_node : public avltree_node_base {
        using link_type = avltree_node<T>*;
        T data;
    };

    struct avltree_iterator_base {
        using base_ptr        = avltree_node_base::base_ptr;
        using difference_type = ptrdiff_t;

        base_ptr m_node;

        void increment() {
            if (m_node->right != nullptr) {
                m_node = avltree_node_base::s_minimum(m_node->right);
            }
            else {
                base_ptr y = m_node->parent;
                while (m_node == y->right) {
                    m_node = y;
                    y      = y->parent;
                }
                if (m_node->right != y) {
                    m_node = y;
                }
            }
        }

        void decrement() {
            if (m_node->color == avltree_red && m_node->parent->parent == m_node) {
                m_node = m_node->right;
            }
            else if (m_node->left != nullptr) {
                m_node = avltree_node_base::s_maximum(m_node->left);
            }
            else {
                base_ptr y = m_node->parent;
                while (m_node == y->left) {
                    m_node = y;
                    y      = y->parent;
                }
                m_node = y;
            }
        }
    };

    template <typename T, typename Ref, typename Ptr>
    struct avltree_iterator : public avltree_iterator_base {
        using value_type     = T;
        using reference      = Ref;
        using pointer        = Ptr;
        using iterator       = avltree_iterator<T, T&, T*>;
        using const_iterator = avltree_iterator<T, const T&, const T*>;

        using link_type = avltree_node<T>*;
        using self      = avltree_iterator<T, Ref, Ptr>;

        avltree_iterator() = default;
        avltree_iterator(link_type x)
            : m_node(x) {}
        avltree_iterator(const iterator& other)
            : m_node(other.m_node) {}

        reference operator*() const { return static_cast<link_type>(m_node)->data; }
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
        self& operator--() {
            decrement();
            return *this;
        }
        self operator--(int) {
            self temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const self& other) const { return m_node == other.m_node; }
        bool operator!=(const self& other) const { return m_node != other.m_node; }
    };

    // Update the height of an AVL-tree node `x` by its 2 childrens.
    inline void update_height(avltree_node_base*& x) {
        x->height = std::max(x->left->height, x->right->height) + (size_t)1;
    }

    // The rotate left operation on an AVL-tree node `x`.
    //     |         -->       |
    //     x         -->       y
    //    /  \       -->      / \
    //   a    y      -->     x   c
    //       / \     -->    / \  |
    //      b   c    -->   a   b X'
    //          |    -->
    //          X'   -->
    inline void avltree_rotate_left(avltree_node_base*& x) {
        avltree_node_base* y = x->right;
        avltree_node_base* b = y->left;

        x->right = b;
        y->left  = x;

        if (b != nullptr)
            b->parent = x;
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->parent = x->parent;
        x->parent = y;

        update_height(x);
        update_height(y);

        x = y;
    }

    // The rotate right operation on an AVL-tree node `x`.
    //       |     -->     |
    //       x     -->     y
    //      / \    -->    / \
    //     y   c   -->   a   x
    //    / \      -->   |  / \
    //   a   b     -->   X'b   c
    //   |         -->
    //   X'        -->
    inline void avltree_rotate_right(avltree_node_base*& x) {
        avltree_node_base* y = x->left;
        avltree_node_base* b = y->right;

        x->left  = b;
        y->right = x;

        if (b != nullptr)
            b->parent = x;
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->parent = x->parent;
        x->parent = y;

        update_height(x);
        update_height(y);

        x = y;
    }

    // The double rotate left operation on an AVL-tree node `x`.
    //       |     -->         |     -->       |
    //       x     -->         x     -->       z
    //      / \    -->        / \    -->      / \
    //     y   c   -->       z   c   -->     y   x
    //    / \      -->      /        -->    / \   \
    //   a   z     -->     y         -->   a   b   c
    //      /      -->    / \        -->
    //     b       -->   a   b       -->
    inline void avltree_double_rotate_left(avltree_node_base*& x) {
        avltree_rotate_left(x->left);
        avltree_rotate_right(x);
    }

    // The double rotate right operation on an AVL-tree node `x`.
    //     |       -->     |         -->       |
    //     x       -->     x         -->       z
    //    / \      -->    / \        -->      / \
    //   a   y     -->   a   z       -->     x   y
    //      / \    -->      / \      -->    / \   \
    //     z   c   -->     b   y     -->   a   b   c
    //    /        -->          \    -->
    //   b         -->           c   -->
    inline void avltree_double_rotate_right(avltree_node_base*& x) {
        avltree_rotate_right(x->right);
        avltree_rotate_left(x);
    }

    inline void avltree_balance(avltree_node_base*& x) {
        if (x == nullptr)
            return;

        if (x->left->height - x->right->height > avltree_balance_factor) {
            if (x->left->left->height >= x->left->right->height) {
                avltree_rotate_right(x);
            }
            else {
                avltree_double_rotate_left(x);
            }
        }
        else if (x->right->height - x->left->height > avltree_balance_factor) {
            if (x->right->right->height >= x->right->left->height) {
                avltree_rotate_left(x);
            }
            else {
                avltree_double_rotate_right(x);
            }
        }
    }

    template <typename T>
    struct avltree_base {
        avltree_base() { m_sentinel = get_node(); }
        ~avltree_base() { put_node(m_sentinel); }

    protected:
        avltree_node<T>* m_sentinel;

        avltree_node<T>* get_node() { return static_cast<avltree_node<T>*>(malloc(sizeof(avltree_node<T>))); }
        void put_node(avltree_node<T>* node) { free(node); }
    };

    template <typename Key, typename Value, typename KeyOfValue, typename Compare>
    class avltree : protected avltree_base<Value> {
        using base = avltree_base<Value>;

    protected:
        using base_ptr     = avltree_node_base*;
        using avltree_node = avltree_node<Value>;
        using height_type  = avltree_height_type;

    public:
        using key_type        = Key;
        using value_type      = Value;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using link_type       = avltree_node*;

    protected:
        // Total number of nodes in the tree.
        size_type m_node_count;
        // A functor.
        Compare m_key_compare;

        link_type create_node(const T& value) {
            link_type temp = get_node();
            try {
                new (&temp->data) T(value);
            }
            catch (const std::exception&) {
                put_node(temp);
                throw;
            }
            return temp;
        }

        link_type clone_node(link_type node) {
            link_type temp = create_node(node->data);
            temp->height   = node->height;
            temp->left     = nullptr;
            temp->right    = nullptr;
            return temp;
        }

        void destroy_node(link_type node) {
            node->data->~T();
            put_node(node);
        }

    protected:
        // The parent of the sentinel node.
        link_type& root() const { return (link_type&)m_sentinel->parent; }
        // The left of the sentinel node.
        link_type& leftmost() const { return (link_type&)m_sentinel->left; }
        // The right of the sentinel node.
        link_type& rightmost() const { return (link_type&)m_sentinel->right; }

        static link_type& s_left(link_type node) { return (link_type&)node->left; }
        static link_type& s_right(link_type node) { return (link_type&)node->right; }
        static link_type& s_parent(link_type node) { return (link_type&)node->parent; }
        static reference s_value(link_type node) { return node->data; }
        static const Key& s_key(link_type node) { return KeyOfValue()(s_value(node)); }
        static height_type& s_height(link_type node) { node == nullptr ? (size_t)-1 : node->height; }

        static link_type& s_left(base_ptr node) { return (link_type&)node->left; }
        static link_type& s_right(base_ptr node) { return (link_type&)node->right; }
        static link_type& s_parent(base_ptr node) { return (link_type&)node->parent; }
        static reference s_value(base_ptr node) { return link_type(node)->data; }
        static const Key& s_key(base_ptr node) { return KeyOfValue()(s_value(link_type(node))); }
        static height_type& s_height(base_ptr node) { node == nullptr ? (size_t)-1 : link_type(node)->height; }

        static link_type s_minimum(link_type node) { return (link_type)avltree_node_base::s_minimum(node); }
        static link_type s_maximum(link_type node) { return (link_type)avltree_node_base::s_maximum(node); }

    public:
        using iterator       = avltree_iterator<value_type, reference, pointer>;
        using const_iterator = avltree_iterator<value_type, const_reference, const_pointer>;

    public:
        avltree()
            : m_node_count(0)
            , m_key_compare() {
            avltree_initialize();
        }

        explicit avltree(const Compare& comp)
            : m_node_count(0)
            , m_key_compare(comp) {
            avltree_initialize();
        }

        avltree(const avltree& other)
            : m_node_count(other.m_node_count)
            , m_key_compare(other.m_key_compare) {
            if (other.root() == nullptr) {
                avltree_initialize();
            }
            else {
                root()      = avltree_copy(other.root(), m_sentinel);
                leftmost()  = s_minimum(root());
                rightmost() = s_maximum(root());
            }
        }

        avltree& operator=(const avltree& other) {
            if (this != &other) {
                clear();
                m_node_count  = other.m_node_count;
                m_key_compare = other.m_key_compare;
                if (other.root() == nullptr) {
                    root()      = nullptr;
                    leftmost()  = m_sentinel;
                    rightmost() = m_sentinel;
                }
                else {
                    root()      = avltree_copy(other.root(), m_sentinel);
                    leftmost()  = s_minimum(root());
                    rightmost() = s_maximum(root());
                }
            }
            return *this;
        }

        ~avltree() { clear(); }

    public:
        Compare key_comp() const { return m_key_compare; }
        iterator begin() { return iterator(leftmost()); }
        iterator end() { return m_sentinel; }
        const_iterator begin() const { return const_iterator(leftmost()); }
        const_iterator end() const { return m_sentinel; }
        bool empty() const { return m_node_count == 0; }
        size_t size() const { return m_node_count; }

        void swap(avltree& other) {
            std::swap(m_sentinel, other.m_sentinel);
            std::swap(m_node_count, other.m_node_count);
            std::swap(m_key_compare, other.m_key_compare);
        }

        bool operator==(const avltree& other) const {
            return (m_node_count == other.m_node_count) && std::equal(begin(), end(), other.begin());
        }

        bool operator!=(const avltree& other) const {
            return !(*this == other);
        }

        bool operator<(const avltree& other) const {
            return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }

    private:
        void avltree_initialize() {
            s_height(m_sentinel) = (height_type)-1;
            root()               = nullptr;
            leftmost()           = m_sentinel;
            rightmost()          = m_sentinel;
        }

        link_type avltree_copy(link_type src, link_type dst) {
            link_type top = clone_node(src);
            top->parent   = dst;

            try {
                if (src->right != nullptr) {
                    s_right(top) = avltree_copy(s_right(src), top);
                }
                dst = top;
                src = s_left(src);

                while (src != nullptr) {
                    link_type y = clone_node(src);
                    s_left(dst) = y;
                    s_parent(y) = dst;
                    if (src->right != nullptr) {
                        s_right(y) = avltree_copy(s_right(src), y);
                    }
                    dst = y;
                    src = s_left(src);
                }
            }
            catch (const std::exception&) {
                avltree_erase(top);
                throw;
            }

            return top;
        }

        void insert_aux(const value_type& value, link_type& parent_node, link_type& current_node) {
            if (current_node == nullptr) {
                current_node           = create_node(value);
                s_parent(current_node) = parent_node;
                s_left(current_node)   = nullptr;
                s_right(current_node)  = nullptr;
                s_height(current_node) = 0;
                ++node_count;
            }
            else if (m_key_compare(value, s_value(current_node)))
                insert_aux(value, current_node, s_left(current_node));
            else if (m_key_compare(s_value(current_node), value))
                insert_aux(value, current_node, s_right(current_node));

            avltree_balance(current_node);
        }

        void erase_aux(const value_type& value, link_type& x) {
            if (x == nullptr)
                return;

            if (m_key_compare(value, s_value(x)))
                erase_aux(value, s_left(x));
            else if (m_key_compare(s_value(x), value))
                erase_aux(value, s_right(x));
            else if (s_left(x) != nullptr && s_right(x) != nullptr) {
                // If both are not null, get the minimum of right subtree.
                s_value(x) = s_value(s_minimum(x->right));
                erase_aux(s_value(x), s_right(x));
            }
            else {
                // If only one subtree, then this one gets promoted.
                link_type old_node = x;
                if (s_left(x) != nullptr)
                    x = s_left(x);
                else
                    x = s_right(x);

                if (x != nullptr)
                    s_parent(x) = s_parent(old_node);
                destroy_node(old_node);
                put_node(old_node);
                --node_count;
            }

            avltree_balance(x);
        }

        void clear_aux(link_type x) {
            while (x != nullptr) {
                clear_aux(s_right(x));
                link_type y = s_parent(x);
                destroy_node(x);
                x = y;
            }
        }

    public:
        void insert(const value_type& value) {
            insert_aux(value, m_sentinel, root());
        }

        void erase(const value_type& value) {
            erase_aux(value, root());
        }

        void clear(link_type& node) {
            if (m_node_count != 0) {
                clear_aux(root());
                root()       = nullptr;
                leftmost()   = m_sentinel;
                rightmost()  = m_sentinel;
                m_node_count = 0;
            }
        }

        iterator find(const Key& key) const {
            iterator i = find(node_value);
            return (i == end() || m_key_compare(key, s_key(i.node))) ? end() : i;
        }

        const_iterator find(const Key& key) const {
            const_iterator i = find(node_value);
            return (i == end() || m_key_compare(key, s_key(i.node))) ? end() : i;
        }

        iterator lower_bound(const Key& key) {
            link_type x = root();
            link_type y = m_sentinel;

            while (x != nullptr) {
                if (!m_key_compare(value(x), key)) {
                    y = x;
                    x = left(x);
                }
                else {
                    x = right(x);
                }
            }

            return iterator(y);
        }

        const_iterator lower_bound(const Key& key) const {
            link_type x = root();
            link_type y = m_sentinel;

            while (x != nullptr) {
                if (!m_key_compare(value(x), key)) {
                    y = x;
                    x = left(x);
                }
                else {
                    x = right(x);
                }
            }

            return const_iterator(y);
        }

        iterator upper_bound(const Key& key) {
            link_type x = root();
            link_type y = m_sentinel;

            while (x != nullptr) {
                if (m_key_compare(key, value(x))) {
                    y = x;
                    x = left(x);
                }
                else {
                    x = right(x);
                }
            }

            return iterator(y);
        }

        const_iterator upper_bound(const Key& key) const {
            link_type x = root();
            link_type y = m_sentinel;

            while (x != nullptr) {
                if (m_key_compare(key, value(x))) {
                    y = x;
                    x = left(x);
                }
                else {
                    x = right(x);
                }
            }

            return const_iterator(y);
        }

        std::pair<iterator, iterator> equal_range(const Key& key) {
            return std::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
        }

        std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
            return std::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
        }
    };

} // namespace Algorithm

#endif //! _ALGORITHM_AVL_TREE_HPP_
