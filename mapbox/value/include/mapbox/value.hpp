#pragma once

#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <mapbox/recursive_wrapper.hpp>
#include <mapbox/variant.hpp>

namespace mapbox {
namespace base {

class Value;

namespace internal {

using ValueBase =
    util::variant<bool, uint64_t, int64_t, double, std::string, util::recursive_wrapper<std::vector<Value>>,
                  util::recursive_wrapper<std::unordered_map<std::string, Value>>>;

} // namespace internal

class Value : public internal::ValueBase {
public:
    using Array = std::vector<Value>;
    using Object = std::unordered_map<std::string, Value>;

    // NOLINTNEXTLINE implicit conversions
    Value(bool v) : internal::ValueBase(v) {}
    // NOLINTNEXTLINE implicit conversions
    Value(const char *c) : internal::ValueBase(std::string(c)) {}
    // NOLINTNEXTLINE implicit conversions
    Value(std::string str) : internal::ValueBase(std::move(str)) {}

    template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0,
              typename std::enable_if_t<std::is_signed<T>::value, int> = 0>
    Value(T t) : internal::ValueBase(int64_t(t)) {} // NOLINT implicit conversions

    template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0,
              typename std::enable_if_t<!std::is_signed<T>::value, int> = 0>
    Value(T t) : internal::ValueBase(uint64_t(t)) {} // NOLINT implicit conversions

    template <typename T, typename std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
    Value(T t) : internal::ValueBase(double(t)) {} // NOLINT implicit conversions
    // NOLINTNEXTLINE implicit conversions
    Value(Array array) : internal::ValueBase(std::move(array)) {}
    // NOLINTNEXTLINE implicit conversions
    Value(Object object) : internal::ValueBase(std::move(object)) {}
};

} // namespace base
} // namespace mapbox
