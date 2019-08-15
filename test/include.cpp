#include <mapbox/geometry.hpp>
#include <mapbox/variant.hpp>

#include <args.hxx>
#include <optional.hpp>

int main() {
    args::ArgumentParser argumentParser("Mapbox Base Test");
    mapbox::geometry::geometry<int> geometry;
    std::experimental::optional<int> optional;
    mapbox::util::variant<int> variant;

    (void)argumentParser;
    (void)geometry;
    (void)optional;
    (void)variant;

    return 0;
}
