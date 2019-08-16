#include <mapbox/geometry.hpp>
#include <mapbox/variant.hpp>

#include <args.hxx>
#include <optional.hpp>
#include <nonstd/expected.hpp>
#include <ghc/filesystem.hpp>

int main() {
    args::ArgumentParser argumentParser("Mapbox Base Test");
    nonstd::expected<void,int> expected;
    mapbox::geometry::geometry<int> geometry;
    std::experimental::optional<int> optional;
    mapbox::util::variant<int> variant;
    ghc::filesystem::path path;

    (void)argumentParser;
    (void)expected;
    (void)geometry;
    (void)optional;
    (void)variant;
    (void)path;

    return 0;
}
