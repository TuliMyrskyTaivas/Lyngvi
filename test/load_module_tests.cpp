/////////////////////////////////////////////////////////////////////////
/// file: tests/load_module_tests.cpp
///
/// summary: Tests for LoadModule function of Lyngvi's API
/////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <lyngvi.h>

TEST(LoadModule, Load)
{    
    EXPECT_NO_THROW(lyngvi::LoadModule("feed_util"));
}
