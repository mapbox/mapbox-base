#include "../mapbox/io/include/mapbox/io.hpp"

#include "io_delete.hpp"

#include <cassert>
#include <string>

int main() {
    ghc::filesystem::path path(std::string(TEST_BINARY_PATH) + "foo.txt");
    ghc::filesystem::path copyPath(std::string(TEST_BINARY_PATH) + "bar.txt");
    ghc::filesystem::path invalidPath("invalid");
    ghc::filesystem::path unauthorizedPath("/root/unauthorized");

    std::string bar("bar");

    nonstd::expected<void, std::string> voidExpected = mapbox::base::writeFile(path, bar);
    assert(voidExpected);

    voidExpected = mapbox::base::writeFile(unauthorizedPath, bar);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to write file '/root/unauthorized'"));

    nonstd::expected<std::string, std::string> stringExpected = mapbox::base::readFile(path);
    assert(stringExpected);
    assert(*stringExpected == bar);

    stringExpected = mapbox::base::readFile(invalidPath);
    assert(!stringExpected);
    assert(stringExpected.error() == std::string("Failed to read file 'invalid'"));

    voidExpected = mapbox::base::copyFile(path, copyPath);
    assert(voidExpected);

    stringExpected = mapbox::base::readFile(copyPath);
    assert(*stringExpected == bar);

    voidExpected = mapbox::base::copyFile(path, unauthorizedPath);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to write file '/root/unauthorized'"));

    voidExpected = mapbox::base::copyFile(invalidPath, path);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to read file 'invalid'"));

    deleteTests(path, copyPath, invalidPath);
    return 0;
}
