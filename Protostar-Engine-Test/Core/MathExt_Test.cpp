#include "pch.h"
#include "MathExt.h"

TEST(MathExt_Test, BasicMath)
{
	EXPECT_TRUE(Protostar::FloatCompare(0.323532, 0.323532));
}