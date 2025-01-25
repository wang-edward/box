#include "core/app.hh"
#include "core/interface.hh"
#include "core/util.hh"
#include "raylib.h"
#include <chrono>
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <random>

#include "test_util.hh"

TEST(ExampleTest, BasicAssertions)
{
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}
