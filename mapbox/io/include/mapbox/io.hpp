#pragma once

#include <ghc/filesystem.hpp>
#include <nonstd/expected.hpp>

#include <string>

namespace mapbox {
namespace base {
namespace io {

using ErrorType = std::string;

nonstd::expected<std::string, ErrorType> readFile(const ghc::filesystem::path& filename);

nonstd::expected<void, ErrorType> writeFile(const ghc::filesystem::path& filename, const std::string& data);

nonstd::expected<void, ErrorType> deleteFile(const ghc::filesystem::path& filename);

nonstd::expected<void, ErrorType> copyFile(const ghc::filesystem::path& sourcePath,
                                           const ghc::filesystem::path& destinationPath);

} // namespace io
} // namespace base
} // namespace mapbox
