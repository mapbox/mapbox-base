#pragma once

#include <mapbox/feature.hpp>

namespace mapbox {
namespace base {

using Value = feature::value;
using ValueArray = Value::array_type;
using ValueObject = Value::object_type;

using null_value_t = mapbox::feature::null_value_t;

} // namespace base
} // namespace mapbox
