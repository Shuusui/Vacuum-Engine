#include "pch.h"
#include "MathExt.h"
#include "Guid.h"

TEST(MathExt_Test, FloatCompare)
{
	//Float compare for floats
	EXPECT_TRUE(Protostar::FloatCompare(0.0000001f, 0.0000001f));
	EXPECT_TRUE(Protostar::FloatCompare(0.0000000000001f, 0.0000000000001f));
	EXPECT_TRUE(Protostar::FloatCompare(0.323532f, 0.323532f));
	EXPECT_TRUE(Protostar::FloatCompare(0.3235322347832345f, 0.3235322347832345f));
	EXPECT_TRUE(Protostar::FloatCompare(0.32353224f, 0.32353223f));
	EXPECT_TRUE(Protostar::FloatCompare(0.3235322347832345f, 0.3235322347832346f));
	EXPECT_FALSE(Protostar::FloatCompare(0.00000000001f, 0.00000000000f));
	EXPECT_FALSE(Protostar::FloatCompare(0.32523f, 0.01231251f));
	EXPECT_FALSE(Protostar::FloatCompare(-0.324232f, 0.324232f));
	//Float compare for double
	EXPECT_TRUE(Protostar::FloatCompare(0.000000000000001, 0.000000000000001));
	EXPECT_TRUE(Protostar::FloatCompare(0.0000000000000001, 0.0000000000000001));
	EXPECT_TRUE(Protostar::FloatCompare(0.323532, 0.323532));
	EXPECT_TRUE(Protostar::FloatCompare(0.3235322347832345, 0.3235322347832346));
	EXPECT_FALSE(Protostar::FloatCompare(0.000000000000001, 0.000000000000000));
	EXPECT_FALSE(Protostar::FloatCompare(0.32523, 0.01231251));
	EXPECT_FALSE(Protostar::FloatCompare(0.32523, 0.01231251));
	EXPECT_FALSE(Protostar::FloatCompare(-0.324232, 0.324232));
}

TEST(Guid_Test, Guid)
{
	using namespace Protostar;
	Guid guid = Guid("d4d1e188312b43e2af7d972acecbd0fc");
	EXPECT_TRUE(guid.IsValid());
	EXPECT_STREQ("d4d1e188312b43e2af7d972acecbd0fc", guid.ToString().c_str());
	EXPECT_STREQ("d4d1e188-312b-43e2-af7d-972acecbd0fc", guid.ToString(EGuidFormats::DigitsWithHyphens).c_str());
	EXPECT_STREQ("{d4d1e188-312b-43e2-af7d-972acecbd0fc}", guid.ToString(EGuidFormats::DigitsWithHyphensInBraces).c_str());
	EXPECT_STREQ("(d4d1e188-312b-43e2-af7d-972acecbd0fc)", guid.ToString(EGuidFormats::DigitsWithHyphensInParentheses).c_str());

	guid = Guid(guid.ToString(EGuidFormats::DigitsWithHyphens), EGuidFormats::DigitsWithHyphens);
	EXPECT_STREQ("d4d1e188312b43e2af7d972acecbd0fc", guid.ToString().c_str());
	EXPECT_STREQ("d4d1e188-312b-43e2-af7d-972acecbd0fc", guid.ToString(EGuidFormats::DigitsWithHyphens).c_str());
	EXPECT_STREQ("{d4d1e188-312b-43e2-af7d-972acecbd0fc}", guid.ToString(EGuidFormats::DigitsWithHyphensInBraces).c_str());
	EXPECT_STREQ("(d4d1e188-312b-43e2-af7d-972acecbd0fc)", guid.ToString(EGuidFormats::DigitsWithHyphensInParentheses).c_str());

	guid = Guid(guid.ToString(EGuidFormats::DigitsWithHyphensInBraces), EGuidFormats::DigitsWithHyphensInBraces);
	EXPECT_STREQ("d4d1e188312b43e2af7d972acecbd0fc", guid.ToString().c_str());
	EXPECT_STREQ("d4d1e188-312b-43e2-af7d-972acecbd0fc", guid.ToString(EGuidFormats::DigitsWithHyphens).c_str());
	EXPECT_STREQ("{d4d1e188-312b-43e2-af7d-972acecbd0fc}", guid.ToString(EGuidFormats::DigitsWithHyphensInBraces).c_str());
	EXPECT_STREQ("(d4d1e188-312b-43e2-af7d-972acecbd0fc)", guid.ToString(EGuidFormats::DigitsWithHyphensInParentheses).c_str());

	guid = Guid(guid.ToString(EGuidFormats::DigitsWithHyphensInParentheses), EGuidFormats::DigitsWithHyphensInParentheses);
	EXPECT_STREQ("d4d1e188312b43e2af7d972acecbd0fc", guid.ToString().c_str());
	EXPECT_STREQ("d4d1e188-312b-43e2-af7d-972acecbd0fc", guid.ToString(EGuidFormats::DigitsWithHyphens).c_str());
	EXPECT_STREQ("{d4d1e188-312b-43e2-af7d-972acecbd0fc}", guid.ToString(EGuidFormats::DigitsWithHyphensInBraces).c_str());
	EXPECT_STREQ("(d4d1e188-312b-43e2-af7d-972acecbd0fc)", guid.ToString(EGuidFormats::DigitsWithHyphensInParentheses).c_str());
	guid.Invalidate();
	EXPECT_FALSE(guid.IsValid());
	guid = Guid::NewGuid();
	EXPECT_TRUE(guid.IsValid());
	Guid guid2 = guid;
	EXPECT_TRUE(guid == guid2);
	EXPECT_FALSE(guid != guid2);
	Guid guid3 = Guid::NewGuid();
	EXPECT_FALSE(guid == guid3);
	EXPECT_TRUE(guid != guid3);
}