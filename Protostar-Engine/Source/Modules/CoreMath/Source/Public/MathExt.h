#pragma once

#include <limits>
#include <cmath>
#include "GlobalDefinitions.h"
#include "CoreMath.h"
#include "GlobalMacros.h"

#ifdef min
#undef min 
#define mindef
#endif

#ifdef max
#undef max
#define maxdef
#endif

namespace Protostar
{
	namespace Core
	{
		template<class T>
		static typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type FloatCompare(const T _a, const T _b, const u32 _ulp = 8)
		{
			return std::fabs(_a - _b) <= std::numeric_limits<T>::epsilon() * std::fabs(_a + _b) * _ulp
				|| std::fabs(_a - _b) < std::numeric_limits<T>::min();
		}

		template<typename T> 
		struct NumericLimits
		{
			PE_NODISCARD static constexpr T Min() noexcept
			{
				return std::numeric_limits<T>::min();
			}
			PE_NODISCARD static constexpr T Max() noexcept
			{
				return std::numeric_limits<T>::max();
			}
			PE_NODISCARD static constexpr T Lowest() noexcept
			{
				return std::numeric_limits<T>::lowest();
			}
			PE_NODISCARD static constexpr T Epsilon() noexcept
			{
				return std::numeric_limits<T>::epsilon();
			}
			PE_NODISCARD static constexpr T RoundError() noexcept
			{
				return std::numeric_limits<T>::round_error();
			}
			PE_NODISCARD static constexpr T DenormMin() noexcept
			{
				return std::numeric_limits<T>::denorm_min();
			}
			PE_NODISCARD static constexpr T Infinity() noexcept
			{
				return std::numeric_limits<T>::infinity();
			}
			PE_NODISCARD static constexpr T QuietNaN() noexcept
			{
				return std::numeric_limits<T>::quiet_NaN();
			}
			PE_NODISCARD static constexpr T SignalingNaN() noexcept
			{
				return std::numeric_limits<T>::signaling_NaN();
			}
		};
	}
}

#ifdef mindef
	#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#ifdef maxdef
	#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif