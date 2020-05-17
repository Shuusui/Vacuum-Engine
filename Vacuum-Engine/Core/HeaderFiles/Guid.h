#pragma once


#pragma region Includes

#pragma region Internal Includes
#include "GlobalDefs.h"
#pragma endregion Internal Includes

#pragma region External Includes
#include <Windows.h>
#include <system_error>
#pragma endregion External Includes

#pragma endregion Includes



namespace Vacuum
{
	namespace Core
	{
		//this is just unreal engine like implementation of a GUID which uses windows provided Guid function to create it but uses own functions for make it usable as keys for maps etc. 
		struct SGuid
		{
		public:
			uint32 A;
			uint32 B;
			uint32 C;
			uint32 D;

			SGuid()
				:A(0)
				,B(0)
				,C(0)
				,D(0)
			{
			}

			SGuid(const SGuid& _other)
				:A(_other.A)
				, B(_other.B)
				, C(_other.C)
				, D(_other.D)
			{
			}

			SGuid(SGuid&& _other) noexcept
				:A(std::move(_other.A))
				,B(std::move(_other.B))
				,C(std::move(_other.C))
				,D(std::move(_other.D))
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
}
namespace std
{
	template<> struct std::hash<Vacuum::Core::SGuid>
	{
		std::size_t operator()(Vacuum::Core::SGuid _guid) const
		{
			const uint64* guidPtr = reinterpret_cast<const uint64*>(&_guid);
			std::hash<uint64> hash;
			return hash(guidPtr[0]) ^ hash(guidPtr[1]);
		}
	};
}