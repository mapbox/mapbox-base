#include <mapbox/io.hpp>

#include "io_delete.hpp"

#include <cassert>
#include <string>

void deleteTests(const std::string& path, const std::string& copyPath, const std::string& invalidPath) {
    nonstd::expected<void, std::string> voidExpected;

    voidExpected = mapbox::base::io::deleteFile(path);
    assert(voidExpected);

    voidExpected = mapbox::base::io::deleteFile(copyPath);
    assert(voidExpected);

    voidExpected = mapbox::base::io::deleteFile(invalidPath);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to delete file 'invalid'"));
}
