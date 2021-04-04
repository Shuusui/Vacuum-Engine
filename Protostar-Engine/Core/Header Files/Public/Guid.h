#pragma once

#pragma region Includes

#pragma region Internal Includes
#include "GlobalDefs.h"
#include "GlobalMacros.h"
#pragma endregion Internal Includes

#pragma region External Includes
#include <Windows.h>
#include <system_error>
#include <sstream>
#pragma endregion External Includes

#pragma endregion Includes


namespace Protostar
{
	enum class EGuidFormats
	{
		Digits,
		DigitsWithHyphens, 
		DigitsWithHyphensInBraces,
		DigitsWithHyphensInParentheses
	};

	//this is just unreal engine like implementation of a GUID which uses windows provided Guid function to create it but uses own functions for make it usable as keys for maps etc. 
	OBJECT()
	struct SGuid
	{
	public:
		PROPERTY()
		u32 A;
		PROPERTY()
		u32 B;
		PROPERTY()
		u32 C;
		PROPERTY()
		u32 D;

		SGuid()
			: A(0)
			, B(0)
			, C(0)
			, D(0)
		{
		}

		SGuid(const std::string& _guidStr, const EGuidFormats _format = EGuidFormats::Digits)
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
			case EGuidFormats::Digits:
				aSubStr = _guidStr.substr(0, 8);
				bFirstStr = _guidStr.substr(8, 4);
				bSecondStr = _guidStr.substr(12, 4);
				cFirstStr = _guidStr.substr(16, 4);
				cSecondStr = _guidStr.substr(20, 4);
				dSubStr = _guidStr.substr(24, 8);
				break;
			case EGuidFormats::DigitsWithHyphens:
				aSubStr = _guidStr.substr(0,8);
				bFirstStr = _guidStr.substr(9, 4);
				bSecondStr = _guidStr.substr(14, 4);
				cFirstStr = _guidStr.substr(19, 4);
				cSecondStr = _guidStr.substr(24, 4);
				dSubStr = _guidStr.substr(28, 8);
				break;
			case EGuidFormats::DigitsWithHyphensInParentheses:
			case EGuidFormats::DigitsWithHyphensInBraces:
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

		SGuid(const SGuid& _other)
			: A(_other.A)
			, B(_other.B)
			, C(_other.C)
			, D(_other.D)
		{
		}

		SGuid(SGuid&& _other) noexcept
			: A(std::move(_other.A))
			, B(std::move(_other.B))
			, C(std::move(_other.C))
			, D(std::move(_other.D))
		{
		}

		void Invalidate()
		{
			A = B = C = D = 0;
		}

		bool IsValid() const
		{
			return (A | B | C | D) != 0;
		}

		std::string ToString(const EGuidFormats _format = EGuidFormats::Digits) const 
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
			case EGuidFormats::Digits:
				resultStream << aStream.str() << bFirstStream.str() << bSecondStream.str() << cStream.str() << dStream.str();
				break;
			case EGuidFormats::DigitsWithHyphens:
				resultStream << aStream.str() << "-" << bFirstStream.str() << "-" << bSecondStream.str() << "-" << cStream.str() << "-" << dStream.str();
				break;
			case EGuidFormats::DigitsWithHyphensInBraces:
				resultStream << "{" << aStream.str() << "-" << bFirstStream.str() << "-" << bSecondStream.str() << "-" << cStream.str() << "-" << dStream.str() << "}";
				break;
			case EGuidFormats::DigitsWithHyphensInParentheses:
				resultStream << "(" << aStream.str() << "-" << bFirstStream.str() << "-" << bSecondStream.str() << "-" << cStream.str() << "-" << dStream.str() << ")";
				break;
			}
			return resultStream.str();
		}

		static SGuid NewGuid()
		{
			SGuid returnGuid;
			HRESULT result = CoCreateGuid((GUID*)&returnGuid);
			if (result != S_OK)
			{
				return SGuid();
			}
			return returnGuid;
		}

		void operator=(const SGuid& _other)
		{
			A = _other.A;
			B = _other.B;
			C = _other.C;
			D = _other.D;
		}

		void operator=(SGuid&& _other) noexcept
		{
			A = std::move(_other.A);
			B = std::move(_other.B);
			C = std::move(_other.C);
			D = std::move(_other.D);
		}

		friend bool operator==(const SGuid& _first, const SGuid& _second)
		{
			return ((_first.A ^ _second.A) | (_first.B ^ _second.B) | (_first.C ^ _second.C) | (_first.D ^ _second.D)) == 0;
		}

		friend bool operator!=(const SGuid& _first, const SGuid& _second)
		{
			return ((_first.A ^ _second.A) | (_first.B ^ _second.B) | (_first.C ^ _second.C)| (_first.D ^ _second.D)) != 0;
		}

		friend bool operator <(const SGuid& _first, const SGuid& _second)
		{
			return	((_first.A < _second.A) ? true : ((_first.A > _second.A)? false :
					((_first.B < _second.B) ? true : ((_first.B > _second.B)? false : 
					((_first.C < _second.C) ? true : ((_first.C > _second.C)? false : 
					((_first.D < _second.D) ? true : ((_first.D > _second.D)? false : false))))))));
		}
	protected:

	private:

	};
}
namespace std
{
	template<> struct hash<Protostar::SGuid>
	{
		size_t operator()(Protostar::SGuid _guid) const
		{
			const u64* guidPtr = reinterpret_cast<const u64*>(&_guid);
			hash<u64> hash;
			return hash(guidPtr[0]) ^ hash(guidPtr[1]);
		}
	};
}