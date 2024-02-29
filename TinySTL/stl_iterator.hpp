#ifndef _TINYSTL_ITERATOR_HPP_
#define _TINYSTL_ITERATOR_HPP_

#include <cstddef>
#include <iostream>

namespace TinySTL {

    // Tags are tagged for function overloaded resolution.
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    // In the case that we lost something.
    template <
        typename Category,
        typename T,
        typename Distance  = ptrdiff_t,
        typename Pointer   = T*,
        typename Reference = T&>
    struct iterator {
        using iterator_category = Category;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = Pointer;
        using reference         = Reference;
    };

    // Read only.
    template <typename T, typename Distance>
    struct input_iterator : public iterator<input_iterator_tag, T> {};

    // Write only.
    struct output_iterator {
        using iterator_category = output_iterator_tag;
        using value_type        = void;
        using difference_type   = void;
        using pointer           = void;
        using reference         = void;
    };

    // Read only.
    template <typename T, typename Distance>
    struct forward_iterator : public iterator<forward_iterator_tag, T> {};

    // Read only.
    template <typename T, typename Distance>
    struct bidirectional_iterator : public iterator<bidirectional_iterator_tag, T> {};

    // Read only.
    template <typename T, typename Distance>
    struct random_access_iterator : public iterator<random_access_iterator_tag, T> {};

    // Using partial specialization.
    // Iterator traits getting information of iterators,
    // regarding pointer as a kind of iterator.
    template <typename I>
    struct iterator_traits {
        using iterator_category = typename I::iterator_category;
        using value_type        = typename I::value_type;
        using difference_type   = typename I::difference_type;
        using pointer           = typename I::pointer;
        using reference         = typename I::reference;
    };

    // For pointer.
    template <typename T>
    struct iterator_traits<T*> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    };

    // For const pointer.
    template <typename T>
    struct iterator_traits<const T*> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;
    };

    // Determine iterator category.
    template <typename I>
    constexpr typename iterator_traits<I>::iterator_category __iterator_category(const I&) {
        using category = typename iterator_traits<I>::iterator_category;
        return category();
    }

    // Determine iterator distance type.
    template <typename I>
    constexpr typename iterator_traits<I>::difference_type* __distance_type(const I&) {
        return static_cast<typename iterator_traits<I>::difference_type*>(0);
    }

    // Determine iterator value type.
    template <typename I>
    constexpr typename iterator_traits<I>::value_type* __value_type(const I&) {
        return static_cast<typename iterator_traits<I>::value_type*>(0);
    }

    // Determine iterator category.
    template <typename I>
    constexpr typename iterator_traits<I>::iterator_category iterator_category(const I& it) {
        return __iterator_category(it);
    }

    // Determine iterator distance type.
    template <typename I>
    constexpr typename iterator_traits<I>::difference_type* distance_type(const I& it) {
        return __distance_type(it);
    }

    // Determine iterator value type.
    template <typename I>
    constexpr typename iterator_traits<I>::value_type* value_type(const I& it) {
        return __value_type(it);
    }

    // For input iterator.
    template <typename I>
    inline typename iterator_traits<I>::difference_type
    __distance(I first, I last, input_iterator_tag) {
        typename iterator_traits<I>::difference_type n = 0;
        while (first != last) {
            first++;
            n++;
        }
        return n;
    }

    // For random access iterator.
    template <typename I>
    inline typename iterator_traits<I>::difference_type
    __distance(I first, I last, random_access_iterator_tag) {
        return last - first;
    }

    // Distance of variant iterator type
    template <typename I>
    inline typename iterator_traits<I>::difference_type
    distance(I first, I last) {
        using category = typename iterator_traits<I>::iterator_category;
        return __distance(first, last, category());
    }

    // For input iterator.
    template <typename I, typename Distance>
    inline void __advance(I& it, Distance n, input_iterator_tag) {
        while (n--)
            it++;
    }

    // For bidirectional.
    template <typename I, typename Distance>
    inline void __advance(I& it, Distance n, bidirectional_iterator_tag) {
        if (n >= 0) {
            while (n--)
                it++;
        }
        else {
            while (n++)
                it--;
        }
    }

    // For random access iterator.
    template <typename I, typename Distance>
    inline void __advance(I& it, Distance n, random_access_iterator_tag) {
        it += n;
    }

    // Advance the iterator, for variant iterator type.
    template <typename I, typename Distance>
    inline void advance(I& it, Distance n) {
        __advance(it, n, iterator_category(it));
    }

    //! Iterator Adapter !//

    // Wrap `push_back` of container as output iterator.
    template <typename Container>
    class back_insert_iterator {
    protected:
        Container* container;

    public:
        // clang-format off
        using container_type    = Container;
        using value_type        = void;
        using pointer           = void;
        using reference         = void;
        using difference_type   = void;
        using iterator_category = output_iterator_tag;
        // clang-format on

        explicit back_insert_iterator(Container& c) noexcept
            : container(&c) {}

        back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
            container->push_back(value);
            return *this;
        }

        // No use, just for standard.
        back_insert_iterator<Container>& operator*() { return *this; }
        back_insert_iterator<Container>& operator++() { return *this; }
        back_insert_iterator<Container> operator++(int) { return *this; }
    };

    template <typename Container>
    inline back_insert_iterator<Container> back_inserter(Container& container) noexcept {
        return back_insert_iterator<Container>(container);
    }

    // Wrap `push_front` of container as output iterator.
    template <typename Container>
    class front_insert_iterator {
    protected:
        Container* container;

    public:
        // clang-format off
        using container_type    = Container;
        using value_type        = void;
        using pointer           = void;
        using reference         = void;
        using difference_type   = void;
        using iterator_category = output_iterator_tag;
        // clang-format on

        explicit front_insert_iterator(Container& c)
            : container(&c) {}

        front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
            container->push_front(value);
            return *this;
        }

        // No use, just for standard.
        front_insert_iterator<Container>& operator*() { return *this; }
        front_insert_iterator<Container>& operator++() { return *this; }
        front_insert_iterator<Container> operator++(int) { return *this; }
    };

    template <typename Container>
    inline front_insert_iterator<Container> front_inserter(Container& container) {
        return front_insert_iterator<Container>(container);
    }

    // Wrap `insert` of container as output iterator.
    template <typename Container>
    class insert_iterator {
    protected:
        Container* container;
        typename Container::iterator iter;

    public:
        // clang-format off
        using container_type    = Container;
        using value_type        = void;
        using pointer           = void;
        using reference         = void;
        using difference_type   = void;
        using iterator_category = output_iterator_tag;
        // clang-format on

        insert_iterator(Container& c, typename Container::iterator it)
            : container(&c), iter(it) {}

        insert_iterator<Container>& operator=(const typename Container::value_type& value) {
            // Insert at the `iter`.
            iter = container->insert(iter, value);
            // Move to next position, otherwise the next insert will be at the same position.
            ++iter;
            return *this;
        }

        // No use, just for standard.
        insert_iterator<Container>& operator*() { return *this; }
        insert_iterator<Container>& operator++() { return *this; }
        insert_iterator<Container> operator++(int) { return *this; }
    };

    template <typename Container>
    inline insert_iterator<Container> inserter(Container& container, typename Container::iterator it) {
        return insert_iterator<Container>(container, it);
    }

    // Wrap `operator<<` of ostream as input iterator.
    template <typename T, typename Diff = ptrdiff_t>
    class istream_iterator {
    protected:
        std::istream* is;
        T value;
        bool end_marker;

        void read() {
            end_marker = (*is) ? true : false;
            if (end_marker) {
                *is >> value;
            }
            // After reading, due to the exception or eof, the stream may be invalid.
            end_marker = (*is) ? true : false;
        }

    public:
        // clang-format off
        using value_type        = T;
        using pointer           = const T*;
        using reference         = const T&;
        using difference_type   = Diff;
        using iterator_category = input_iterator_tag;
        // clang-format on

        istream_iterator()
            : is(&std::cin), end_marker(false) {}

        // Once constructed, it will wait for input.
        istream_iterator(std::istream& s)
            : is(&s) { read(); }

        reference operator*() const { return value; }
        pointer operator->() const { return &value; }

        istream_iterator<T>& operator++() {
            read();
            return *this;
        }

        istream_iterator<T> operator++(int) {
            istream_iterator<T> tmp = *this;
            read();
            return tmp;
        }
    };

    // Wrap `operator<<` of ostream as output iterator.
    template <typename T>
    class ostream_iterator {
    protected:
        std::ostream* os;
        const char* str;

    public:
        // clang-format off
        using value_type        = void;
        using pointer           = void;
        using reference         = void;
        using difference_type   = void;
        using iterator_category = output_iterator_tag;
        // clang-format on

        ostream_iterator(std::ostream& s, const char* c = nullptr)
            : os(&s), str(c) {}

        ostream_iterator<T>& operator=(const T& value) {
            *os << value;
            if (str) {
                *os << str;
            }
            return *this;
        }

        // No use, just for standard.
        ostream_iterator<T>& operator*() { return *this; }
        ostream_iterator<T>& operator++() { return *this; }
        ostream_iterator<T> operator++(int) { return *this; }
    };

    template <typename Iterator>
    class reverse_iterator {
    protected:
        Iterator current;

    public:
        // clang-format off
        using iterator_category = typename iterator_traits<Iterator>::iterator_category;
        using value_type        = typename iterator_traits<Iterator>::value_type;
        using pointer           = typename iterator_traits<Iterator>::pointer;
        using reference         = typename iterator_traits<Iterator>::reference;
        using difference_type   = typename iterator_traits<Iterator>::difference_type;
    
        using self = reverse_iterator<Iterator>;
        // clang-format on

    public:
        reverse_iterator() {}

        explicit reverse_iterator(Iterator it) noexcept
            : current(it) {}

        reverse_iterator(const self& it)
            : current(it.current) {}

        reverse_iterator& operator=(const self& it) {
            current = it.current;
            return *this;
        }

        Iterator base() const { return current; }

        reference operator*() const {
            Iterator temp = current;
            return *--temp;
        }
        pointer operator->() const {
            return &(operator*());
        }

        self& operator++() {
            --current;
            return *this;
        }

        self operator++(int) {
            self temp = *this;
            --current;
            return temp;
        }

        self& operator--() {
            ++current;
            return *this;
        }

        self operator--(int) {
            self temp = *this;
            ++current;
            return temp;
        }

        self operator+(difference_type n) const {
            return self(current - n);
        }

        self& operator+=(difference_type n) {
            current -= n;
            return *this;
        }

        self operator-(difference_type n) const {
            return self(current + n);
        }

        self& operator-=(difference_type n) {
            current += n;
            return *this;
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }

        bool operator==(const reverse_iterator& other) const { return this->base() == other.base(); }
        bool operator!=(const reverse_iterator& other) const { return this->base() != other.base(); }
        bool operator<(const reverse_iterator& other) const { return other.base() < this->base(); }

        difference_type operator-(const reverse_iterator& other) const {
            return difference_type(other.base() - this->base());
        }
    };

} // namespace TinySTL

#endif // !_TINYSTL_ITERATOR_HPP_
