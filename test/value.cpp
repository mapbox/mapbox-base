#include "../mapbox/value/include/mapbox/value.hpp"

#include <cassert>

namespace {

template <typename T, typename U>
void checkType(U&& arg) try {
    mapbox::base::Value value{std::forward<U>(arg)};
    assert(value.template is<T>());
    assert(value.template get<T>() == arg);
} catch (...) {
    assert(false);
}

} // namespace

int main() {
    checkType<int64_t>(32);
    checkType<uint64_t>(32u);
    checkType<double>(32.0);
    checkType<bool>(false);
    checkType<std::string>("hello");

    mapbox::base::Value intValue{32};
    try {
        intValue.get<uint64_t>();
        assert(false); // Should throw.
    } catch (...) {
    }

    return 0;
}
