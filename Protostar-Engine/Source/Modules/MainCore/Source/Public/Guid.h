#pragma once

#include "GlobalDefinitions.h"
#include "MainCore.h"
#include <string>

namespace Protostar::Core
{
	enum class GuidFormats
	{
		Digits,
		DigitsWithHyphens,
		DigitsWithHyphensInBraces,
		DigitsWithHyphensInParentheses
	};

	//this is just unreal engine like implementation of a GUID which uses windows provided Guid function to create it but uses own functions for make it usable as keys for maps etc. 
	struct MAINCORE_API Guid
	{
	public:
		u32 A;
		u32 B;
		u32 C;
		u32 D;

		Guid()
			: A(0)
			, B(0)
			, C(0)
			, D(0)
		{
		}

		Guid(const std::string& _guidStr, const GuidFormats _format = GuidFormats::Digits);

		Guid(const Guid& _other)
			: A(_other.A)
			, B(_other.B)
			, C(_other.C)
			, D(_other.D)
		{
		}

		Guid(Guid&& _other) noexcept
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

		std::string ToString(const GuidFormats _format = GuidFormats::Digits) const;

		static Guid NewGuid();

		void operator=(const Guid& _other)
		{
			A = _other.A;
			B = _other.B;
			C = _other.C;
			D = _other.D;
		}

		void operator=(Guid&& _other) noexcept
		{
			A = std::move(_other.A);
			B = std::move(_other.B);
			C = std::move(_other.C);
			D = std::move(_other.D);
		}

		friend bool operator==(const Guid& _first, const Guid& _second)
		{
			return ((_first.A ^ _second.A) | (_first.B ^ _second.B) | (_first.C ^ _second.C) | (_first.D ^ _second.D)) == 0;
		}

		friend bool operator!=(const Guid& _first, const Guid& _second)
		{
			return ((_first.A ^ _second.A) | (_first.B ^ _second.B) | (_first.C ^ _second.C) | (_first.D ^ _second.D)) != 0;
		}

		friend bool operator <(const Guid& _first, const Guid& _second)
		{
			return	((_first.A < _second.A) ? true : ((_first.A > _second.A) ? false :
				((_first.B < _second.B) ? true : ((_first.B > _second.B) ? false :
					((_first.C < _second.C) ? true : ((_first.C > _second.C) ? false :
						((_first.D < _second.D) ? true : ((_first.D > _second.D) ? false : false))))))));
		}
	protected:

	private:

	};
}

namespace std
{
	template<> struct hash<Protostar::Core::Guid>
	{
		size_t operator()(Protostar::Core::Guid _guid) const
		{
			const u64* guidPtr = reinterpret_cast<const u64*>(&_guid);
			hash<u64> hash;
			return hash(guidPtr[0]) ^ hash(guidPtr[1]);
		}
	};
}
