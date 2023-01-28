#ifndef _TINYSTL_QUEUE_HPP_
#define _TINYSTL_QUEUE_HPP_

#include <stl_deque.hpp>
#include <stl_function.hpp>
#include <stl_heap.hpp>
#include <stl_vector.hpp>

namespace TinySTL {

    template <typename T, typename Container = deque<T>>
    class queue {
    public:
        using value_type      = typename Container::value_type;
        using size_type       = typename Container::size_type;
        using reference       = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using container_type  = Container;

    protected:
        Container container;

    public:
        queue()
            : container() {}
        explicit queue(const Container& other)
            : container(other) {}
        ~queue() = default;

        bool empty() const { return container.empty(); }
        size_type size() const { return container.size(); }
        reference front() { return container.front(); }
        const_reference front() const { return container.front(); }
        reference back() { return container.back(); }
        const_reference back() const { return container.back(); }

        bool operator==(const queue& other) const { return container == other.container; }
        bool operator!=(const queue& other) const { return container != other.container; }
        bool operator<(const queue& other) const { return container < other.container; }

        void push(const value_type& value) { container.push_back(value); }
        void pop() { container.pop_front(); }
    };

    template <typename T, typename Container = vector<T>, typename Compare = less<typename Container::value_type>>
    class priority_queue {
    public:
        using value_type      = typename Container::value_type;
        using size_type       = typename Container::size_type;
        using reference       = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using container_type  = Container;

    protected:
        Container container;
        Compare comp;

    public:
        priority_queue()
            : container() {}

        explicit priority_queue(const Compare& c)
            : container(), comp(c) {}

        priority_queue(const Compare& c, const Container& s)
            : container(s), comp(c) {
            make_heap(container.begin(), container.end(), comp);
        }

        template <typename InputIterator>
        priority_queue(const InputIterator first, const InputIterator last)
            : container(first, last) {
            make_heap(container.begin(), container.end(), comp);
        }

        template <typename InputIterator>
        priority_queue(const InputIterator first, const InputIterator last, const Compare& c)
            : container(first, last), comp(c) {
            make_heap(container.begin(), container.end(), comp);
        }

        template <typename InputIterator>
        priority_queue(const InputIterator first, const InputIterator last, const Compare& c, const Container& s)
            : container(s), comp(c) {
            container.insert(container.end(), first, last);
            make_heap(container.begin(), container.end(), comp);
        }

        bool empty() const { return container.empty(); }
        size_type size() const { return container.size(); }
        const_reference top() const { return container.front(); }

        void push(const value_type& value) {
            try {
                container.push_back(value);
                push_heap(container.begin(), container.end(), comp);
            }
            catch (const std::exception&) {
                container.clear();
            }
        }

        void pop() {
            try {
                pop_heap(container.begin(), container.end(), comp);
                container.pop_back();
            }
            catch (const std::exception&) {
                container.clear();
            }
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_QUEUE_HPP_
