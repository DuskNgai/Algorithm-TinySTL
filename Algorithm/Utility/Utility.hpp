#ifndef _ALGORITHM_UTILITY_HPP_
#define _ALGORITHM_UTILITY_HPP_

#include <functional>

namespace Algorithm {

    template <typename T>
    std::function<bool(T const&, T const&)> EqualTwoContainer = [](T const& container1, T const& container2) {
        return std::equal(
            container1.begin(), container1.end(), container2.begin(), container2.end()
        );
    };

} // namespace Algorithm

#endif // !_ALGORITHM_UTILITY_HPP_