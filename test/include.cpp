#include <mapbox/geometry.hpp>
#include <mapbox/variant.hpp>

#include <optional.hpp>

int main() {
    mapbox::geometry::geometry<int> geometry;
    std::experimental::optional<int> optional;
    mapbox::util::variant<int> variant;

    (void)geometry;
    (void)optional;
    (void)variant;

    return 0;
}
