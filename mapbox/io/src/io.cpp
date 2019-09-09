#include <mapbox/io.hpp>

#include <cerrno>
#include <fstream>
#include <iostream>
#include <sstream>

namespace mapbox {
namespace base {
namespace io {

nonstd::expected<std::string, ErrorType> readFile(const std::string& filename) {
    nonstd::expected<std::string, ErrorType> result;

    std::ifstream file(filename, std::ios::binary);
    if (!file.good()) {
        return nonstd::make_unexpected(std::string("Failed to read file '") + filename + std::string("'"));
    }

    std::stringstream data;
    data << file.rdbuf();
    result = data.str();
    return result;
}

nonstd::expected<void, ErrorType> writeFile(const std::string& filename, const std::string& data) {
    nonstd::expected<void, ErrorType> result;

    std::ofstream file(filename, std::ios::binary);
    if (!file.good()) {
        return nonstd::make_unexpected(std::string("Failed to write file '") + filename + std::string("'"));
    }

    file << data;

    return result;
}

nonstd::expected<void, ErrorType> deleteFile(const std::string& filename) {
    nonstd::expected<void, ErrorType> result;

    const int ret = std::remove(filename.c_str());
    if (ret != 0) {
        return nonstd::make_unexpected(std::string("Failed to delete file '") + filename + std::string("'"));
    }

    return result;
}

nonstd::expected<void, ErrorType> copyFile(const std::string& sourcePath, const std::string& destinationPath) {
    auto contents = readFile(sourcePath);
    if (!contents) {
        return nonstd::make_unexpected(contents.error());
    }

    return writeFile(destinationPath, *contents);
}

} // namespace io
} // namespace base
} // namespace mapbox
