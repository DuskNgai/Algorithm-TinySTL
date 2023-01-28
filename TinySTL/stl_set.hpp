#ifndef _TINYSTL_SET_HPP_
#define _TINYSTL_SET_HPP_

#include <stl_function.hpp>
#include <stl_tree.hpp>

namespace TinySTL {

    template <typename Key, typename Compare = TinySTL::less<Key>, typename Alloc = alloc>
    class set {
    public:
        using key_type      = Key;
        using value_type    = Key;
        using key_compare   = Compare;
        using value_compare = Compare;

    private:
        using rep_type = rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc>;
        rep_type t;

    public:
        using pointer         = typename rep_type::const_pointer;
        using const_pointer   = typename rep_type::const_pointer;
        using reference       = typename rep_type::const_reference;
        using const_reference = typename rep_type::const_reference;
        using iterator        = typename rep_type::const_iterator;
        using const_iterator  = typename rep_type::const_iterator;
        using size_type       = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;
        using allocator_type  = typename rep_type::allocator_type;

        set()
            : t(Compare(), allocator_type()) {}

        explicit set(const Compare& comp, const allocator_type& a = allocator_type())
            : t(comp, a) {}

        set(const value_type* first, const value_type* last)
            : t(Compare(), allocator_type()) {
            t.insert_unique(first, last);
        }

        set(const value_type* first, const value_type* last, const Compare& comp, const allocator_type& a = allocator_type())
            : t(comp, a) {
            t.insert_unique(first, last);
        }

        set(const_iterator first, const_iterator last)
            : t(Compare(), allocator_type()) {
            t.insert_unique(first, last);
        }

        set(const_iterator first, const_iterator last, const Compare& comp, const allocator_type& a = allocator_type())
            : t(comp, a) {
            t.insert_unique(first, last);
        }

        set(const set& x)
            : t(x.t) {}

        set& operator=(const set& x) {
            t = x.t;
            return *this;
        }

    public:
        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return t.key_comp(); }

        iterator begin() const { return t.begin(); }
        iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }

        void swap(set& x) { t.swap(x.t); }

        TinySTL::pair<iterator, bool> insert(const value_type& value) {
            TinySTL::pair<typename rep_type::iterator, bool> p = t.insert_unique(value);
            return TinySTL::pair<iterator, bool>(p.first, p.second);
        }

        iterator insert(iterator position, const value_type& value) {
            using rep_iterator = typename rep_type::iterator;
            return t.insert_unique((rep_iterator&)position, value);
        }

        void insert(const value_type* first, const value_type* last) {
            t.insert_unique(first, last);
        }

        void insert(const_iterator first, const_iterator last) {
            t.insert_unique(first, last);
        }

        void erase(iterator position) {
            using rep_iterator = typename rep_type::iterator;
            t.erase((rep_iterator&)position);
        }

        size_type erase(const key_type& key) {
            return t.erase(key);
        }

        void erase(iterator first, iterator last) {
            using rep_iterator = typename rep_type::iterator;
            t.erase((rep_iterator&)first, (rep_iterator&)last);
        }

        void clear() { t.clear(); }

        iterator find(const key_type& key) const { return t.find(key); }
        size_type count(const key_type& key) const { return t.count(key); }
        iterator lower_bound(const key_type& key) const { return t.lower_bound(key); }
        iterator upper_bound(const key_type& key) const { return t.upper_bound(key); }
        TinySTL::pair<iterator, iterator> equal_range(const key_type& key) const { return t.equal_range(key); }

        bool operator==(const set& other) const { return t == other.t; }
        bool operator<(const set& other) const { return t < other.t; }
    };

} // namespace TinySTL

#endif // !_TINYSTL_SET_HPP_
