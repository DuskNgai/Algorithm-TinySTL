#ifndef _TINYSTL_STACK_HPP_
#define _TINYSTL_STACK_HPP_

#include <stl_deque.hpp>

namespace TinySTL {

    template <typename T, typename Container = deque<T>>
    class stack {
    public:
        using value_type      = typename Container::value_type;
        using size_type       = typename Container::size_type;
        using reference       = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using container_type  = Container;

    protected:
        Container container;

    public:
        stack()
            : container() {}
        explicit stack(const Container& other)
            : container(other) {}
        ~stack() = default;

        bool empty() const { return container.empty(); }
        size_type size() const { return container.size(); }
        reference top() { return container.back(); }
        const_reference top() const { return container.back(); }

        bool operator==(const stack& other) const { return container == other.container; }
        bool operator!=(const stack& other) const { return container != other.container; }
        bool operator<(const stack& other) const { return container < other.container; }

        void push(const value_type& value) { container.push_back(value); }
        void pop() { container.pop_back(); }
    };

} // namespace TinySTL

#endif // !_TINYSTL_STACK_HPP_
