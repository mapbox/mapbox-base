#pragma once

#include <functional>

// Need std=c++17 or higher.
#if (__cplusplus > 201402L && __has_include(<optional>))
#include <optional>
#else
#include "impl/_optional.hpp"

namespace mapbox {
namespace base {

template <typename T>
using optional = std::experimental::optional<T>;

using bad_optional_access = std::experimental::bad_optional_access;

constexpr std::experimental::nullopt_t nullopt{std::experimental::nullopt_t::init()};
constexpr std::experimental::trivial_init_t trivial_init{};
constexpr std::experimental::in_place_t in_place{};

template <class X>
inline constexpr std::experimental::optional<X&> make_optional(std::reference_wrapper<X> v) {
    return std::experimental::make_optional(v);
}

template <class T>
inline constexpr std::experimental::optional<typename std::decay<T>::type> make_optional(T&& v) {
    return std::experimental::optional<typename std::decay<T>::type>(std::experimental::constexpr_forward<T>(v));
}

// workaround: std utility functions aren't constexpr yet
template <class T>
inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type& t) noexcept {
    return std::experimental::constexpr_forward(t);
}

template <class T>
inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type&& t) noexcept {
    return std::experimental::constexpr_forward(std::move(t));
}

template <class T>
inline constexpr typename std::remove_reference<T>::type&& constexpr_move(T&& t) noexcept {
    return std::experimental::constexpr_move(std::move(t));
}

} // namespace base
} // namespace mapbox
#endif
