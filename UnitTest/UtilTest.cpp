#include <gtest/gtest.h>
#include "../Source/UtilFunc.h"

using namespace hks;

TEST(UtilFunc, ClampTest)
{
	EXPECT_EQ(0, Clamp(-10, 0, 10));
	EXPECT_EQ(0, Clamp(0, 0, 10));
	EXPECT_EQ(10, Clamp(10, 0, 10));
	EXPECT_EQ(10, Clamp(20, 0, 10));
}

TEST(UtilFunc, WrapTest)
{
	EXPECT_EQ(5, Wrap(15, 0, 10));
	EXPECT_EQ(5, Wrap(-5, 0, 10));
}

TEST(UtilFunc, RangeTest)
{
	EXPECT_TRUE(InRange(0, 0, 10));
	EXPECT_TRUE(InRange(5, 0, 10));
	EXPECT_TRUE(InRange(10, 0, 10));
	EXPECT_FALSE(InRange(-10, 0, 10));
	EXPECT_FALSE(InRange(20, 0, 10));
}
