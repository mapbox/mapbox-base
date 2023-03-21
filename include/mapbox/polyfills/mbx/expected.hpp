#pragma once

#include <nonstd/expected.hpp>

namespace mbx {

template <typename T, typename E>
using expected = nonstd::expected<T, E>;

template <typename E>
auto make_unexpected(E&& value) {
    return nonstd::make_unexpected<E>(std::forward<E>(value));
}

} // namespace mbx
