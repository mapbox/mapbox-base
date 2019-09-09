#include <ghc/filesystem.hpp>
#include <mapbox/io.hpp>

#include "io_delete.hpp"

#include <cassert>
#include <string>

int main() {
    ghc::filesystem::path path(std::string(TEST_BINARY_PATH) + "foo.txt");
    ghc::filesystem::path copyPath(std::string(TEST_BINARY_PATH) + "bar.txt");
    ghc::filesystem::path invalidPath("invalid");
    ghc::filesystem::path unauthorizedPath("/root/unauthorized");

    std::string bar("bar");

    nonstd::expected<void, std::string> voidExpected = mapbox::base::io::writeFile(path.string(), bar);
    assert(voidExpected);

    voidExpected = mapbox::base::io::writeFile(unauthorizedPath, bar);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to write file '/root/unauthorized'"));

    nonstd::expected<std::string, std::string> stringExpected = mapbox::base::io::readFile(path.string());
    assert(stringExpected);
    assert(*stringExpected == bar);

    stringExpected = mapbox::base::io::readFile(invalidPath);
    assert(!stringExpected);
    assert(stringExpected.error() == std::string("Failed to read file 'invalid'"));

    voidExpected = mapbox::base::io::copyFile(path.string(), copyPath);
    assert(voidExpected);

    stringExpected = mapbox::base::io::readFile(copyPath);
    assert(*stringExpected == bar);

    voidExpected = mapbox::base::io::copyFile(path.string(), unauthorizedPath);
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to write file '/root/unauthorized'"));

    voidExpected = mapbox::base::io::copyFile(invalidPath, path.string());
    assert(!voidExpected);
    assert(voidExpected.error() == std::string("Failed to read file 'invalid'"));

    deleteTests(path.string(), copyPath, invalidPath);
    return 0;
}
