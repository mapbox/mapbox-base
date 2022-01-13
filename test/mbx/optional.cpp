#include "mbx/optional"

#include <gtest/gtest.h>

TEST(Optional, Optional) {
    mbx::optional<bool> optionalBoolean;
    EXPECT_FALSE(optionalBoolean);

    optionalBoolean = { false };
    EXPECT_TRUE(optionalBoolean);
    EXPECT_FALSE(optionalBoolean.value());
    EXPECT_FALSE(optionalBoolean.operator *());
}
