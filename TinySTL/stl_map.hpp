#ifndef _TINYSTL_MAP_HPP_
#define _TINYSTL_MAP_HPP_

#include <stl_function.hpp>
#include <stl_tree.hpp>

namespace TinySTL {

    template <typename Key, typename Value, typename Compare = TinySTL::less<Key>, typename Alloc = alloc>
    class map {
    public:
        using key_type    = Key;
        using data_type   = Value;
        using mapped_type = Value;
        using value_type  = TinySTL::pair<const Key, Value>;
        using key_compare = Compare;

        class value_compare : public binary_function<value_type, value_type, bool> {
            friend class map<Key, Value, Compare, Alloc>;

        protected:
            Compare comp;
            value_compare(Compare c)
                : comp(c) {}

        public:
            bool operator()(const value_type& x, const value_type& y) const {
                return comp(x.first, y.first);
            }
        };

    private:
        using rep_type = rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc>;
        rep_type t;

    public:
        using pointer         = typename rep_type::pointer;
        using const_pointer   = typename rep_type::const_pointer;
        using reference       = typename rep_type::reference;
        using const_reference = typename rep_type::const_reference;
        using iterator        = typename rep_type::iterator;
        using const_iterator  = typename rep_type::const_iterator;
        using size_type       = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;
        using allocator_type  = typename rep_type::allocator_type;

        map()
            : t(Compare(), allocator_type()) {}

        explicit map(const Compare& comp, const allocator_type& a = allocator_type())
            : t(comp, a) {}

        map(const value_type* first, const value_type* last)
            : t(Compare(), allocator_type()) {
            t.insert_unique(first, last);
        }

        map(const value_type* first, const value_type* last, const Compare& comp, const allocator_type& a = allocator_type())
            : t(comp, a) {
            t.insert_unique(first, last);
        }

        map(const_iterator first, const_iterator last)
            : t(Compare(), allocator_type()) {
            t.insert_unique(first, last);
        }

        map(const_iterator first, const_iterator last, const Compare& comp, const allocator_type& a = allocator_type())
            : t(comp, a) {
            t.insert_unique(first, last);
        }

        map(const map& x)
            : t(x.t) {}

        map operator=(const map& x) {
            t = x.t;
            return *this;
        }

    public:
        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return value_compare(t.key_comp()); }

        iterator begin() { return t.begin(); }
        iterator end() { return t.end(); }
        const_iterator begin() const { return t.begin(); }
        const_iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }

        Value& operator[](const key_type& k) {
            iterator i = lower_bound(k);
            // If not found, insert a new `value_type()`.
            if (i == end() || key_comp()(k, (*i).first))
                i = insert(i, value_type(k, Value()));
            return (*i).second;
        }
        void swap(map& x) { t.swap(x.t); }

        TinySTL::pair<iterator, bool> insert(const value_type& x) {
            return t.insert_unique(x);
        }

        iterator insert(iterator position, const value_type& x) {
            return t.insert_unique(position, x);
        }

        void insert(const value_type* first, const value_type* last) {
            t.insert_unique(first, last);
        }

        void insert(const_iterator first, const_iterator last) {
            t.insert_unique(first, last);
        }

        void erase(iterator position) {
            t.erase(position);
        }

        size_type erase(const key_type& x) {
            return t.erase(x);
        }

        void erase(iterator first, iterator last) {
            t.erase(first, last);
        }

        void clear() { t.clear(); }

        iterator find(const key_type& x) { return t.find(x); }
        const_iterator find(const key_type& x) const { return t.find(x); }
        size_type count(const key_type& x) const { return t.count(x); }
        iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
        const_iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
        iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
        const_iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
        TinySTL::pair<iterator, iterator> equal_range(const key_type& x) { return t.equal_range(x); }
        TinySTL::pair<const_iterator, const_iterator> equal_range(const key_type& x) const { return t.equal_range(x); }

        friend bool operator==(const map& lhs, const map& rhs) noexcept { return lhs.t == rhs.t; }
        friend bool operator!=(const map& lhs, const map& rhs) noexcept { return !(lhs == rhs); }
        friend bool operator<(const map& lhs, const map& rhs) noexcept { return lhs.t < rhs.t; }
        friend bool operator>(const map& lhs, const map& rhs) noexcept { return rhs < lhs; }
        friend bool operator<=(const map& lhs, const map& rhs) noexcept { return !(rhs < lhs); }
        friend bool operator>=(const map& lhs, const map& rhs) noexcept { return !(lhs < rhs); }
    };

} // namespace TinySTL

#endif // !_TINYSTL_MAP_HPP_
