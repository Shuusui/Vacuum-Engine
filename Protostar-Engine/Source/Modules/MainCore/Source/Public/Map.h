#pragma once
#include "Iterator.h"
#include "Allocator.h"
#include <functional>

namespace Protostar::Core
{
	template<typename T1, typename T2> 
	struct Pair
	{

	};

	template<typename KEY, typename T, typename HASH = std::hash<KEY>, 
		typename KEYEQUAL = std::equal_to<KEY>, typename ALLOCATOR = Allocator<Pair<const KEY, T>>>
	class HashMap
	{
		explicit HashMap(std::size_t _hashBucketCount,
			const HASH& _hash = HASH(), const KEYEQUAL& _equal = KEYEQUAL(),
			const ALLOCATOR& _allocator = ALLOCATOR());
	};

}