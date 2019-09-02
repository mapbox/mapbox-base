#include "../mapbox/io/include/mapbox/io.hpp"

#include <cassert>

int main() {
    ghc::filesystem::path path(std::string(TEST_BINARY_PATH) + "foo.txt");
    ghc::filesystem::path copyPath(std::string(TEST_BINARY_PATH) + "bar.txt");
    ghc::filesystem::path invalidPath("invalid");
    ghc::filesystem::path unauthorizedPath("/root/unauthorized");

    std::string bar("bar");

    nonstd::expected<void, std::string> voidExpected = mapbox::util::writeFile(path, bar);
    assert(voidExpected);

    voidExpected = mapbox::util::writeFile(unauthorizedPath, bar);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to write file '/root/unauthorized'"));

    nonstd::expected<std::string, std::string> stringExpected = mapbox::util::readFile(path);
    assert(stringExpected);
    assert(*stringExpected == bar);

    stringExpected = mapbox::util::readFile(invalidPath);
    assert(!stringExpected);
    assert(stringExpected.error() == std::string("Failed to read file 'invalid'"));

    voidExpected = mapbox::util::copyFile(path, copyPath);
    assert(voidExpected);

    stringExpected = mapbox::util::readFile(copyPath);
    assert(*stringExpected == bar);

    voidExpected = mapbox::util::copyFile(path, unauthorizedPath);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to write file '/root/unauthorized'"));

    voidExpected = mapbox::util::copyFile(invalidPath, path);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to read file 'invalid'"));

    voidExpected = mapbox::util::deleteFile(path);
    assert(voidExpected);

    voidExpected = mapbox::util::deleteFile(copyPath);
    assert(voidExpected);

    voidExpected = mapbox::util::deleteFile(invalidPath);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to delete file 'invalid'"));

    return 0;
}
