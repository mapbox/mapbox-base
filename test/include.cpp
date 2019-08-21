#include <mapbox/geojson.hpp>
#include <mapbox/geometry.hpp>
#include <mapbox/variant.hpp>
#include <mapbox/pixelmatch.hpp>

#include <args.hxx>
#include <optional.hpp>
#include <nonstd/expected.hpp>
#include <ghc/filesystem.hpp>
#include <rapidjson/document.h>
#include <jni/jni.hpp>
#include <supercluster.hpp>

int main() {
    args::ArgumentParser argumentParser("Mapbox Base Test");
    nonstd::expected<void,int> expected;
    mapbox::geometry::geometry<int> geometry;
    std::experimental::optional<int> optional;
    mapbox::util::variant<int> variant;
    ghc::filesystem::path path;
    jni::jlong jlong;
    mapbox::geojson::geojson geojson;
    mapbox::supercluster::Options superclusterOptions;

    rapidjson::Document rapidjsonDocument;

    (void)argumentParser;
    (void)expected;
    (void)geometry;
    (void)optional;
    (void)variant;
    (void)path;
    (void)rapidjsonDocument;
    (void)jlong;
    (void)geojson;
    (void)superclusterOptions;

    mapbox::pixelmatch(nullptr, nullptr, 0u, 0u, nullptr, 0.0);

    return 0;
}
