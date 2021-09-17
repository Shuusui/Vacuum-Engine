#pragma once
#include "GlobalDefs.h"
#include "Windows.h"
import <limits>;
import <cmath>;

#ifdef min
#undef min 
#define mindef
#endif

namespace Protostar
{
	template<class T>
	static typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type FloatCompare(const T _a, const T _b, const u32 _ulp = 8)
	{
		return std::fabs(_a - _b) <= std::numeric_limits<T>::epsilon() * std::fabs(_a + _b) * _ulp
			|| std::fabs(_a - _b) < std::numeric_limits<T>::min();
	}
}

#ifdef mindef
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif