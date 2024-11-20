#include <gtest/gtest.h>
#include <iostream>
#include "core/util.hh"

TEST(ExampleTest, BasicAssertions) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}
