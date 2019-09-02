#pragma once

#include <string>

#include <ghc/filesystem.hpp>
#include <nonstd/expected.hpp>

#include <cerrno>
#include <fstream>
#include <iostream>
#include <sstream>

namespace mapbox {
namespace base {

using ErrorType = std::string;

inline nonstd::expected<std::string, ErrorType> readFile(const ghc::filesystem::path& filename) {
    nonstd::expected<std::string, ErrorType> result;

    std::ifstream file(filename.string(), std::ios::binary);
    if (!file.good()) {
        return nonstd::make_unexpected(std::string("Failed to read file '") + filename.string() + std::string("'"));
    }

    std::stringstream data;
    data << file.rdbuf();
    result = data.str();
    return result;
}

inline nonstd::expected<void, ErrorType> writeFile(const ghc::filesystem::path& filename, const std::string& data) {
    nonstd::expected<void, ErrorType> result;

    std::ofstream file(filename, std::ios::binary);
    if (!file.good()) {
        return nonstd::make_unexpected(std::string("Failed to write file '") + filename.string() + std::string("'"));
    }

    file << data;

    return result;
}

inline nonstd::expected<void, ErrorType> deleteFile(const ghc::filesystem::path& filename) {
    nonstd::expected<void, ErrorType> result;

    const int ret = std::remove(filename.string().c_str());
    if (ret != 0) {
        return nonstd::make_unexpected(std::string("Failed to delete file '") + filename.string() + std::string("'"));
    }

    return result;
}

inline nonstd::expected<void, ErrorType> copyFile(const ghc::filesystem::path& sourcePath,
                                                  const ghc::filesystem::path& destinationPath) {
    auto contents = readFile(sourcePath);
    if (!contents) {
        return nonstd::make_unexpected(contents.error());
    }

    return writeFile(destinationPath, *contents);
}

} // namespace base
} // namespace mapbox
