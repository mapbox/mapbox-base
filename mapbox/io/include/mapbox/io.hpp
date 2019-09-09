#pragma once

#include <nonstd/expected.hpp>

#include <string>

namespace mapbox {
namespace base {
namespace io {

using ErrorType = std::string;

nonstd::expected<std::string, ErrorType> readFile(const std::string& filename);

nonstd::expected<void, ErrorType> writeFile(const std::string& filename, const std::string& data);

nonstd::expected<void, ErrorType> deleteFile(const std::string& filename);

nonstd::expected<void, ErrorType> copyFile(const std::string& sourcePath, const std::string& destinationPath);

} // namespace io
} // namespace base
} // namespace mapbox
