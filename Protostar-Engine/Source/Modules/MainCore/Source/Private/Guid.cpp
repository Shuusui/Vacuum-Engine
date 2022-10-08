#include "Guid.h"
#include <sstream>

namespace Protostar::Core
{
	Guid::Guid(const std::string& _guidStr, const GuidFormats _format)
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{
		if (_guidStr.empty())
		{
			return;
		}

		std::stringstream stream;
		u16 bFirst = 0;
		u16 bSecond = 0;
		u16 cFirst = 0;
		u16 cSecond = 0;
		std::string aSubStr = {};
		std::string bFirstStr = {};
		std::string bSecondStr = {};
		std::string cFirstStr = {};
		std::string cSecondStr = {};
		std::string dSubStr = {};

		switch (_format)
		{
		case GuidFormats::Digits:
			aSubStr = _guidStr.substr(0, 8);
			bFirstStr = _guidStr.substr(8, 4);
			bSecondStr = _guidStr.substr(12, 4);
			cFirstStr = _guidStr.substr(16, 4);
			cSecondStr = _guidStr.substr(20, 4);
			dSubStr = _guidStr.substr(24, 8);
			break;
		case GuidFormats::DigitsWithHyphens:
			aSubStr = _guidStr.substr(0, 8);
			bFirstStr = _guidStr.substr(9, 4);
			bSecondStr = _guidStr.substr(14, 4);
			cFirstStr = _guidStr.substr(19, 4);
			cSecondStr = _guidStr.substr(24, 4);
			dSubStr = _guidStr.substr(28, 8);
			break;
		case GuidFormats::DigitsWithHyphensInParentheses:
		case GuidFormats::DigitsWithHyphensInBraces:
			aSubStr = _guidStr.substr(1, 8);
			bFirstStr = _guidStr.substr(10, 4);
			bSecondStr = _guidStr.substr(15, 4);
			cFirstStr = _guidStr.substr(20, 4);
			cSecondStr = _guidStr.substr(25, 4);
			dSubStr = _guidStr.substr(29, 8);
			break;
		}

		stream << std::hex << aSubStr.c_str();
		stream >> A;
		stream = {};
		stream << std::hex << bFirstStr.c_str();
		stream >> bFirst;
		stream = {};
		stream << std::hex << bSecondStr.c_str();
		stream >> bSecond;
		stream = {};
		stream << std::hex << cFirstStr.c_str();
		stream >> cFirst;
		stream = {};
		stream << std::hex << cSecondStr.c_str();
		stream >> cSecond;
		stream = {};
		stream << std::hex << dSubStr.c_str();
		stream >> D;
		B = bSecond;
		B |= bFirst << 16;
		C = cSecond;
		C |= cFirst << 16;
	}

	std::string Guid::ToString(const GuidFormats _format) const
	{
		if (!IsValid())
		{
			return std::string();
		}

		u16 bFirst = B >> 16;
		u16 bSecond = 0;
		bSecond |= B;
		u16 cFirst = C >> 16;
		u16 cSecond = 0;
		cSecond |= C;
		u64 dTemp = ((u64)cSecond) << 32;
		dTemp |= D;
		std::stringstream aStream;
		std::stringstream bFirstStream;
		std::stringstream bSecondStream;
		std::stringstream cStream;
		std::stringstream dStream;

		if (A <= 0xFFFFFFF)
		{
			aStream << '0';
		}

		if (bFirst <= 0xFFF)
		{
			bFirstStream << '0';
		}

		if (bSecond <= 0xFFF)
		{
			bSecondStream << '0';
		}

		if (cFirst < 0xFFF)
		{
			cStream << '0';
		}

		if (dTemp < 0xFFFFFFFFFFF)
		{
			dStream << '0';
		}

		aStream << std::hex << A;
		bFirstStream << std::hex << bFirst;
		bSecondStream << std::hex << bSecond;
		cStream << std::hex << cFirst;
		dStream << std::hex << dTemp;
		std::stringstream resultStream;

		switch (_format)
		{
		case GuidFormats::Digits:
			resultStream << aStream.str() << bFirstStream.str() << bSecondStream.str() << cStream.str() << dStream.str();
			break;
		case GuidFormats::DigitsWithHyphens:
			resultStream << aStream.str() << "-" << bFirstStream.str() << "-" << bSecondStream.str() << "-" << cStream.str() << "-" << dStream.str();
			break;
		case GuidFormats::DigitsWithHyphensInBraces:
			resultStream << "{" << aStream.str() << "-" << bFirstStream.str() << "-" << bSecondStream.str() << "-" << cStream.str() << "-" << dStream.str() << "}";
			break;
		case GuidFormats::DigitsWithHyphensInParentheses:
			resultStream << "(" << aStream.str() << "-" << bFirstStream.str() << "-" << bSecondStream.str() << "-" << cStream.str() << "-" << dStream.str() << ")";
			break;
		}
		return resultStream.str();
	}
}