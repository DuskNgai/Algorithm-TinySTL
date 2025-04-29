#ifndef _TINYSTL_PAIR_HPP_
#define _TINYSTL_PAIR_HPP_

namespace TinySTL {

    template <typename T1, typename T2>
    struct pair {
        using first_type  = T1;
        using second_type = T2;

        T1 first{};
        T2 second{};
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
