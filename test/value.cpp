#include "../mapbox/value/include/mapbox/value.hpp"

#include <cassert>

namespace {

template <typename T, typename U>
void checkType(U&& arg) try {
    mapbox::base::Value value{std::forward<U>(arg)};
    assert(value);
    assert(value.template is<T>());
    assert(value.template get<T>() == arg);
} catch (...) {
    assert(false);
}

} // namespace

int main() {
    assert(!mapbox::base::Value());
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

    mapbox::base::Value arrayValue{{32, "hello", 1.0}};
    assert(arrayValue);
    assert(arrayValue.is<mapbox::base::ValueArray>());
    try {
        const auto& array = arrayValue.get<mapbox::base::ValueArray>();
        assert(array.size() == 3u);
        assert(array[0].get<int64_t>() == 32);
        assert(array[1].get<std::string>() == "hello");
        assert(array[2].get<double>() - 1.0 < 0.00000001);
    } catch (...) {
        assert(false);
    }

    return 0;
}
