#ifndef _TINYSTL_TREE_HPP_
#define _TINYSTL_TREE_HPP_

#include <stl_algorithm.hpp>
#include <stl_allocator.hpp>
#include <stl_iterator.hpp>
#include <stl_pair.hpp>
#include <stl_type_traits.hpp>
#include <stl_uninitialized.hpp>

// Red Black Tree (RBTree) is a balanced tree.
// 1. Every node is red or black.
// 2. The root node is black.
// 3. The children of a red node is black.
// 4. All path from one node to the leaf node contains the same number of black nodes.
// 5. All the leaf nodes should be black.
// 6. New node should be red.
// 7. The parent of the new node should be black.

namespace TinySTL {

    using rbtree_color_type                         = bool;
    static constexpr rbtree_color_type rbtree_red   = false;
    static constexpr rbtree_color_type rbtree_black = true;

    struct rbtree_node_base {
        using color_type = rbtree_color_type;
        using base_ptr   = rbtree_node_base*;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr right;

        static base_ptr s_minimum(base_ptr x) {
            while (x->left != nullptr) {
                x = x->left;
            }
            return x;
        }

        static base_ptr s_maximum(base_ptr x) {
            while (x->right != nullptr) {
                x = x->right;
            }
            return x;
        }
    };

    template <typename T>
    struct rbtree_node : public rbtree_node_base {
        using link_type = rbtree_node<T>*;
        T data;
    };

    struct rbtree_iterator_base {
        using base_ptr          = rbtree_node_base::base_ptr;
        using iterator_category = bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        base_ptr m_node;

        // Go to the next node in the RB-tree.
        void increment() {
            // Case 1: if right child exist, find the left-most node in right subtree.
            if (m_node->right != nullptr) {
                m_node = rbtree_node_base::s_minimum(m_node->right);
            }
            else {
                // Case 2: if it is a right-most node in some subtree (no right child),
                // find a ancestor `y` where `m_node` belongs to its left subtree.
                base_ptr y = m_node->parent;
                while (m_node == y->right) {
                    m_node = y;
                    y      = y->parent;
                }
                // Case 3: if it is not the right-most node in the whole tree,
                // then `y` is the successor of `m_node`.
                // Case 4: otherwise, there is no successor of `m_node`,
                // in this case, `m_node` is now the sentinel node.
                if (m_node->right != y) {
                    m_node = y;
                }
            }
        }

        // Go to the previous node in the RB-tree.
        void decrement() {
            // Case 1: `m_node` is the sentinel node.
            if (m_node->color == rbtree_red && m_node->parent->parent == m_node) {
                m_node = m_node->right;
            }
            // Case 2: if the left child exist, find the right-most node of left subtree.
            else if (m_node->left != nullptr) {
                m_node = rbtree_node_base::s_maximum(m_node->left);
            }
            // Case 3: if it is a left-most node in some subtree (no left child),
            // find a ancestor `y` where `m_node` belongs to its right subtree.
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
    struct rbtree_iterator : public rbtree_iterator_base {
        using value_type     = T;
        using reference      = Ref;
        using pointer        = Ptr;
        using iterator       = rbtree_iterator<T, T&, T*>;
        using const_iterator = rbtree_iterator<T, const T&, const T*>;

        using self      = rbtree_iterator<T, Ref, Ptr>;
        using link_type = rbtree_node<T>*;

        rbtree_iterator() = default;
        rbtree_iterator(link_type x)
            : m_node(x) {}
        rbtree_iterator(const iterator& other)
            : m_node(other.m_node) {}

        reference operator*() const { return link_type(m_node)->data; }
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

        bool operator==(const rbtree_iterator_base& other) {
            return m_node == other.m_node;
        }
        bool operator!=(const rbtree_iterator_base& other) {
            return m_node != other.m_node;
        }
    };

    constexpr ptrdiff_t* distance_type(const rbtree_iterator_base&) {
        return static_cast<ptrdiff_t*>(0);
    }

    template <typename T, typename Ref, typename Ptr>
    constexpr T* value_type(const rbtree_iterator<T, Ref, Ptr>&) {
        return static_cast<T*>(0);
    }

    constexpr bidirectional_iterator_tag iterator_category(const rbtree_iterator_base&) {
        return bidirectional_iterator_tag();
    }

    // The rotate left operation on an RB-tree node `x`.
    // `x` must have a right child.
    // After the operation, the right child of `x` becomes the parent of `x`.
    // The left child of the right child of `x` becomes the right child of `x`.
    // The parent of `x` becomes the left child of the right child of `x`.
    // If `x` is the root node, then the right child of `x` becomes the root node.
    // If `x` is a left child, then the right child of `x` becomes a left child.
    // If `x` is a right child, then the right child of `x` becomes a right child.
    //     R       -->       R
    //     |       -->       |
    //     x       -->       y
    //    / \      -->      / \  
    //   a   y     -->     x   c
    //      / \    -->    /\     
    //     b   c   -->   a  b
    inline void rbtree_rotate_left(rbtree_node_base* x, rbtree_node_base*& root) {
        rbtree_node_base* y = x->right;
        x->right            = y->left;
        // `x` becomes the parent of the subtree's root.
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        if (x == root) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left   = x;
        x->parent = y;
    }

    // The rotate right operation on an RB-tree node `x`.
    // `x` must have a left child.
    // After the operation, the left child of `x` becomes the parent of `x`.
    // The right child of the left child of `x` becomes the left child of `x`.
    // The parent of `x` becomes the right child of the left child of `x`.
    // If `x` is the root node, then the left child of `x` becomes the root node.
    // If `x` is a left child, then the left child of `x` becomes a left child.
    // If `x` is a right child, then the left child of `x` becomes a right child.
    //       R     -->     R
    //       |     -->     |
    //       x     -->     y
    //      / \    -->    / \    
    //     y   c   -->   a   x
    //    / \      -->      / \  
    //   a   b     -->     b   c
    inline void rbtree_rotate_right(rbtree_node_base* x, rbtree_node_base*& root) {
        rbtree_node_base* y = x->left;
        x->left             = y->right;
        // `x` becomes the parent of the subtree's root.
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        if (x == root) {
            root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right  = x;
        x->parent = y;
    }

    inline void rbtree_rebalance_for_insert(rbtree_node_base* x, rbtree_node_base*& root) {
        // The newly inserted node `x` is always red.
        x->color = rbtree_red;
        while (x != root && x->parent->color == rbtree_red) {
            if (x->parent == x->parent->parent->left) {
                // Find the uncle node `y`.
                rbtree_node_base* y = x->parent->parent->right;
                // Uncle node `y` is red, change the color.
                //   |      -->    |
                //   B      -->    R(x)
                //  / \     -->   / \
                // R   R(y) -->  B   B(y)
                // |        -->  |
                // R(x)     -->  R
                if (y != nullptr && y->color == rbtree_red) {
                    x->parent->color         = rbtree_black;
                    y->color                 = rbtree_black;
                    x->parent->parent->color = rbtree_red;
                    // Continue to rebalance the tree from the grandparent node.
                    x = x->parent->parent;
                }
                else {
                    // Uncle node `y` is black, rotate the tree.
                    //   |    -->          |
                    //   B    -->          B
                    //  / \   -->         / \
                    // R   B  --> (old x)R   B
                    //  \     -->       /
                    //   R(x) -->      R(new x)
                    if (x == x->parent->right) {
                        x = x->parent;
                        rbtree_rotate_left(x, root);
                    }
                    //     |   -->     |   -->      |
                    //     B   -->     R   -->      B
                    //    / \  -->    / \  -->     / \
                    //   R   B -->   B   B --> (x)R   R
                    //  /      -->  /      -->         \
                    // R(x)    --> R(x)    -->          B
                    x->parent->color         = rbtree_black;
                    x->parent->parent->color = rbtree_red;
                    rbtree_rotate_right(x->parent->parent, root);
                }
            }
            else {
                rbtree_node_base* y = x->parent->parent->left;
                if (y != nullptr && y->color == rbtree_red) {
                    x->parent->color         = rbtree_black;
                    y->color                 = rbtree_black;
                    x->parent->parent->color = rbtree_red;

                    x = x->parent->parent;
                }
                else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rbtree_rotate_right(x, root);
                    }
                    x->parent->color         = rbtree_black;
                    x->parent->parent->color = rbtree_red;
                    rbtree_rotate_left(x->parent->parent, root);
                }
            }
        }
        root->color = rbtree_black;
    }

    inline void rbtree_rebalance_for_erase(rbtree_node_base* x, rbtree_node_base*& root, rbtree_node_base*& leftmost, rbtree_node_base*& rightmost) {
        
    }

    template <typename T, typename Alloc>
    struct rbtree_base {
        using allocator_type = Alloc;

        rbtree_base(const allocator_type&) { m_sentinel = get_node(); }
        ~rbtree_base() { put_node(m_sentinel); }

    protected:
        typedef simple_alloc<rbtree_node<T>, Alloc> rbtree_node_allocator;

        rbtree_node<T>* m_sentinel;

        rbtree_node<T>* get_node() { return rbtree_node_allocator::allocate(1); }
        void put_node(rbtree_node<T>* node) { rbtree_node_allocator::deallocate(node, 1); }
    };

    //   Root ----
    //   /  \     |
    //  L    R    |
    // / \  / \   |
    // ........   |
    //   \  /     |
    //     S -----
    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
    class rbtree : protected rbtree_base<Value, Alloc> {
        using base = rbtree_base<Value, Alloc>;

    protected:
        using base_ptr    = rbtree_node_base*;
        using rbtree_node = rbtree_node<Value>;
        using color_type  = rbtree_color_type;

    public:
        using key_type        = Key;
        using value_type      = Value;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using link_type       = rbtree_node*;
        using allocator_type  = typename base::allocator_type;

    protected:
        link_type create_node(const value_type& value) {
            link_type temp = get_node();
            try {
                construct(&temp->data, value);
            }
            catch (const std::exception&) {
                put_node(temp);
                throw;
            }
            return temp;
        }

        link_type clone_node(link_type node) {
            link_type temp = create_node(node->data);
            temp->color    = node->color;
            temp->left     = nullptr;
            temp->right    = nullptr;
            return temp;
        }

        void destroy_node(link_type node) {
            destory(&node->data);
            put_node(node);
        }

    protected:
        // Total number of nodes in the tree.
        size_type m_node_count;
        // A functor.
        Compare m_key_compare;

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
        static const key_type& s_key(link_type node) { return KeyOfValue()(s_value(node)); }
        static color_type& s_color(link_type node) { return node->color; }

        static link_type& s_left(base_ptr node) { return (link_type&)node->left; }
        static link_type& s_right(base_ptr node) { return (link_type&)node->right; }
        static link_type& s_parent(base_ptr node) { return (link_type&)node->parent; }
        static reference s_value(base_ptr node) { return link_type(node)->data; }
        static const key_type& s_key(base_ptr node) { return KeyOfValue()(s_value(link_type(node))); }
        static color_type& s_color(base_ptr node) { return link_type(node)->color; }

        static link_type s_minimum(link_type node) { return (link_type)rbtree_node_base::s_minimum(node); }
        static link_type s_maximum(link_type node) { return (link_type)rbtree_node_base::s_maximum(node); }

    public:
        // clang-format off
        using iterator       = rbtree_iterator<value_type, reference, pointer>;
        using const_iterator = rbtree_iterator<value_type, const_reference, const_pointer>;
        // clang-format on

    public:
        rbtree()
            : base(allocator_type())
            , m_node_count(0)
            , m_key_compare() {
            rbtree_initialize();
        }

        explicit rbtree(const Compare& comp)
            : base(allocator_type())
            , m_node_count(0)
            , m_key_compare(comp) {
            rbtree_initialize();
        }

        rbtree(const Compare& comp, const allocator_type& allocator)
            : base(allocator)
            , m_node_count(0)
            , m_key_compare(comp) {
            rbtree_initialize();
        }

        rbtree(const rbtree& other)
            : base(other.get_allocator())
            , m_node_count(other.m_node_count)
            , m_key_compare(other.m_key_compare) {
            if (other.root() == nullptr) {
                rbtree_initialize();
            }
            else {
                s_color(m_sentinel) = rbtree_red;
                root(m_sentinel)    = rbtree_copy(other.root(), m_sentinel);
                leftmost()          = s_minimum(root());
                rightmost()         = s_maximum(root());
            }
        }

        rbtree& operator=(const rbtree& other) {
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
                    root()      = rbtree_copy(other.root(), m_sentinel);
                    leftmost()  = minimum(root());
                    rightmost() = minimum(root());
                }
            }
            return *this;
        }

        ~rbtree() { clear(); }

    public:
        Compare key_comp() const { return m_key_compare; }
        iterator begin() { return iterator(leftmost()); }
        iterator end() { return m_sentinel; }
        const_iterator begin() const { return const_iterator(leftmost()); }
        const_iterator end() const { return m_sentinel; }
        bool empty() const { return m_node_count == 0; }
        size_type size() const { return m_node_count; }

        void swap(rbtree& other) {
            TinySTL::swap(m_sentinel, other.m_sentinel);
            TinySTL::swap(m_node_count, other.m_node_count);
            TinySTL::swap(m_key_compare, other.m_key_compare);
        }

        bool operator==(const rbtree& other) const {
            return (m_node_count == other.m_node_count) && equal(begin(), end(), other.begin());
        }

        bool operator!=(const rbtree& other) const {
            return !(*this == other);
        }

        bool operator<(const rbtree& other) const {
            return lexicographical_compare(begin(), end(), other.begin(), other.end());
        }

    private:
        void rbtree_initialize() {
            s_color(m_sentinel) = rbtree_red;
            root()              = nullptr;
            leftmost()          = m_sentinel;
            rightmost()         = m_sentinel;
        }

        link_type rbtree_copy(link_type x, link_type p) {
            link_type top = clone_node(x);
            top->parent   = p;

            try {
                // Copy the right subtree.
                if (x->right != nullptr) {
                    s_right(top) = rbtree_copy(s_right(x), top);
                }
                // Prepare for the left subtree.
                p = top;
                x = s_left(x);

                // Copy the left subtree.
                while (x != nullptr) {
                    // Make link between parent and left child.
                    link_type y = clone_node(x);
                    s_left(p)   = y;
                    s_parent(y) = p;
                    // Copy the right subtree of left subtree.
                    if (x->right != nullptr) {
                        s_right(y) = rbtree_copy(s_right(x), y);
                    }
                    p = y;
                    x = s_left(x);
                }
            }
            catch (const std::exception&) {
                rbtree_erase(top);
                throw;
            }

            return top;
        }

        iterator insert_aux(base_ptr x, base_ptr y, const value_type& value) {
            link_type xx = (link_type)x;
            // `yy` is the parent of `zz`.
            link_type yy = (link_type)y;
            link_type zz = create_node(value);

            // Insert at the left.
            if (yy == m_sentinel || xx != nullptr || m_key_compare(KeyOfValue()(value), s_key(yy))) {
                s_left(yy) = zz;
                // Tree was empty.
                if (yy == m_sentinel) {
                    root()      = zz;
                    rightmost() = zz;
                }
                else if (yy == leftmost()) {
                    leftmost() = zz;
                }
            }
            // Insert at the right.
            else {
                s_right(yy) = zz;
                if (yy == rightmost()) {
                    rightmost() = zz;
                }
            }

            s_left(zz)   = nullptr;
            s_right(zz)  = nullptr;
            s_parent(zz) = yy;
            rbtree_rebalance_for_insert(zz, root());
            ++m_node_count;
            return iterator(zz);
        }

        void erase_aux(link_type x) {
            while (x != nullptr) {
                // Erase right subtree.
                erase_aux(s_right(x));
                // Switch to left subtree.
                link_type y = s_left(x);
                destroy_node(x);
                x = y;
            }
        }

    public:
        iterator insert_equal(const value_type& value) {
            // Node being compared with `value`.
            link_type x = root();
            // `y` will be parent of node containing `value`.
            link_type y = m_sentinel;
            bool comp   = true;

            while (x != nullptr) {
                y    = x;
                comp = m_key_compare(KeyOfValue()(value), s_key(x));
                x    = comp ? s_left(x) : s_right(x);
            }
            // `x` will always be `nullptr`.
            return insert_aux(x, y, value);
        }

        iterator insert_equal(iterator pos, const value_type& value) {
            if (pos == begin()) {
                // Insert at the left of leftmost, need not find the position to insert.
                if (size() > 0 && m_key_compare(KeyOfValue()(value), s_key(pos.m_node))) {
                    // First argument just needs to be non-null.
                    return insert_aux(pos.m_node, pos.m_node, value);
                }
                else {
                    return insert_equal(value);
                }
            }
            else if (pos == end()) {
                // Insert at the right of rightmost, need not find the position to insert.
                if (!m_key_compare(KeyOfValue()(value), s_key(rightmost()))) {
                    return insert_aux(nullptr, rightmost(), value);
                }
                else {
                    return insert_equal(value);
                }
            }
            else {
                iterator before = pos;
                --before;
                if (!m_key_compare(KeyOfValue()(value), s_key(before.m_node)) && !m_key_compare(s_key(pos.m_node), KeyOfValue()(value))) {
                    // Insert between `before` and `pos`.
                    if (s_right(before.m_node) == nullptr) {
                        return insert_aux(nullptr, before.m_node, value);
                    }
                    else {
                        return insert_aux(pos.m_node, pos.m_node, value);
                    }
                }
                else {
                    return insert_equal(value);
                }
            }
        }

        void insert_equal(const value_type* first, const value_type* last) {
            for (; first != last; ++first) {
                insert_equal(*first);
            }
        }

        void insert_equal(const_iterator first, const_iterator last) {
            for (; first != last; ++first) {
                insert_equal(*first);
            }
        }

        TinySTL::pair<iterator, bool> insert_unique(const value_type& value) {
            link_type x = root();
            link_type y = m_sentinel;
            bool comp   = true;

            while (x != nullptr) {
                y    = x;
                comp = m_key_compare(KeyOfValue()(value), s_key(x));
                x    = comp ? s_left(x) : s_right(x);
            }

            // `i` will be parent of node containing `value`.
            // If there are multiple nodes containing `i`'value, `i` will be the right most one.
            iterator i = iterator(y);
            // If `comp` is true, which means `value` is smaller than `i`'s value,
            // then new node is a left child of `i`.
            if (comp) {
                if (i == begin()) {
                    // Insert at the left of the leftmost node.
                    // `x` will always be `nullptr`.
                    return TinySTL::pair<iterator, bool>(insert_aux(x, y, value), true);
                }
                else {
                    // Find a smaller node than `i`, which may be equal to the given value.
                    --i;
                }
            }
            // Equal check.
            if (m_key_compare(s_key(i.m_node), KeyOfValue()(value))) {
                // `x` will always be `nullptr`.
                return TinySTL::pair<iterator, bool>(insert_aux(x, y, value), true);
            }
            // If equal, insert failed.
            return TinySTL::pair<iterator, bool>(i, false);
        }

        iterator insert_unique(iterator pos, const value_type& value) {
            if (pos == begin()) {
                // Insert at the left of leftmost, need not find the position to insert.
                if (size() > 0 && m_key_compare(KeyOfValue()(value), s_key(pos.m_node))) {
                    // First argument just needs to be non-null.
                    return insert_aux(pos.m_node, pos.m_node, value);
                }
                else {
                    return insert_unique(value).first;
                }
            }
            else if (pos == end()) {
                // Insert at the right of rightmost, need not find the position to insert.
                if (m_key_compare(s_key(rightmost()), KeyOfValue()(value))) {
                    return insert_aux(nullptr, rightmost(), value);
                }
                else {
                    return insert_unique(value).first;
                }
            }
            else {
                iterator before = pos;
                --before;
                if (m_key_compare(s_key(before.m_node), KeyOfValue()(value)) && m_key_compare(KeyOfValue()(value), s_key(pos.m_node))) {
                    // Insert between `before` and `pos`.
                    if (s_right(before.m_node) == nullptr) {
                        return insert_aux(nullptr, before.m_node, value);
                    }
                    else {
                        return insert_aux(pos.m_node, pos.m_node, value);
                    }
                }
                else {
                    return insert_unique(value).first;
                }
            }
        }

        void insert_unique(const value_type* first, const value_type* last) {
            for (; first != last; ++first) {
                insert_unique(*first);
            }
        }

        void insert_unique(const_iterator first, const_iterator last) {
            for (; first != last; ++first) {
                insert_unique(*first);
            }
        }

        void erase(iterator pos) {
            link_type y = static_cast<link_type>(rbtree_rebalance_for_erase(
                pos.m_node, root(), leftmost(), rightmost()
            ));
            destroy_node(y);
            --m_node_count;
        }

        size_type erase(const Key& key) {
            std::pair<iterator, iterator> p = equal_range(key);
            size_type n                     = TinySTL::distance(p.first, p.second);
            erase(p.first, p.second);
            return n;
        }

        void erase(const Key* first, const Key* last) {
            for (; first != last; ++first) {
                erase(*first);
            }
        }

        void erase(const_iterator first, const_iterator last) {
            if (first == begin() && last == end()) {
                clear();
            }
            else {
                for (; first != last; ++first) {
                    erase(first);
                }
            }
        }

        iterator find(const Key& key) {
            iterator i = lower_bound(key);
            // If `i` is end() or `key` is smaller than `i`'s value, return end().
            return (i == end() || m_key_compare(key, s_key(i.m_node))) ? end() : i;
        }

        const_iterator find(const Key& key) const {
            const_iterator i = lower_bound(key);
            return (i == end() || m_key_compare(key, s_key(i.m_node))) ? end() : i;
        }

        size_type count(const Key& key) const {
            TinySTL::pair<const_iterator, const_iterator> p = equal_range(key);
            return distance(p.first, p.second);
        }

        iterator lower_bound(const Key& key) {
            link_type x = root();
            link_type y = m_sentinel;

            // Find the first node whose value is no smaller than `key`.
            while (x != nullptr) {
                if (!m_key_compare(s_key(x), key)) {
                    y = x;
                    x = s_left(x);
                }
                else {
                    x = s_right(x);
                }
            }

            return iterator(y);
        }

        const_iterator lower_bound(const Key& key) const {
            link_type x = root();
            link_type y = m_sentinel;

            while (x != nullptr) {
                if (!m_key_compare(s_key(x), key)) {
                    y = x;
                    x = s_left(x);
                }
                else {
                    x = s_right(x);
                }
            }

            return const_iterator(y);
        }

        iterator upper_bound(const Key& key) {
            link_type x = root();
            link_type y = m_sentinel;

            // Find the first node whose value is greater than `key`.
            while (x != nullptr) {
                if (m_key_compare(key, s_key(x))) {
                    y = x;
                    x = s_left(x);
                }
                else {
                    x = s_right(x);
                }
            }

            return iterator(y);
        }

        const_iterator upper_bound(const Key& key) const {
            link_type x = root();
            link_type y = m_sentinel;

            while (x != nullptr) {
                if (m_key_compare(key, s_key(x))) {
                    y = x;
                    x = s_left(x);
                }
                else {
                    x = s_right(x);
                }
            }

            return const_iterator(y);
        }

        TinySTL::pair<iterator, iterator> equal_range(const Key& key) {
            return TinySTL::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
        }

        TinySTL::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
            return TinySTL::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_TREE_HPP_
