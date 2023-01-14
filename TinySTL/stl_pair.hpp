#ifndef _TINYSTL_PAIR_HPP_
#define _TINYSTL_PAIR_HPP_

namespace TinySTL {

    template <typename T1, typename T2>
    struct pair {
        // clang-format off
        using first_type  = T1;
        using second_type = T2;
        // clang-format on

        T1 first;
        T2 second;

        pair()
            : first(T1()), second(T2()) {}
        pair(const T1& t1, const T2& t2)
            : first(t1), second(t2) {}
        pair(T1&& t1, T2 && t2) noexcept
            : first(t1), second(t2) {}
        ~pair() = default;
    };

    template <typename T1, typename T2>
    inline pair<T1, T2> make_pair(const T1& t1, const T2& t2) {
        return pair<T1, T2>(t1, t2);
    }

    template <typename T1, typename T2>
    inline bool operator==(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
        return p1.first == p2.first && p1.second == p2.second;
    }

} // namespace TinySTL

#endif // !_TINYSTL_PAIR_HPP_
