////////////////////////////////////////////////////////////////////////
/// file: tests/get_text_tests.cpp
///
/// summary: Tests for LoadModule function of Lyngvi's API
/////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <lyngvi.h>

/////////////////////////////////////////////////////////////////////////
TEST(GetText, ExistingText)
{
    const char* const source = "FeedsDir should not be empty";
    const char* const module = "feed_util";
    
    EXPECT_NO_THROW(lyngvi::LoadModule(module));            
    const char* const result = lyngvi::GetText(module, source);
    EXPECT_NE(source, result);
    EXPECT_STREQ(result, source);
}

/////////////////////////////////////////////////////////////////////////
TEST(GetText, NonExistingText)
{    
    const char* const source = "AAAAAAAAAAAAAAAAAAAAAAAA";
    const char* const module = "feed_util";

    EXPECT_NO_THROW(lyngvi::LoadModule(module));
    const char* const result = lyngvi::GetText(module, source);
    EXPECT_EQ(source, result);
    EXPECT_STREQ(result, source);
}