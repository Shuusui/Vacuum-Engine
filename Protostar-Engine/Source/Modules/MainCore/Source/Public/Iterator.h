#pragma once
#include "GlobalDefinitions.h"

namespace Protostar::Core
{
	template<typename T, typename IMP = void>
	class ConstIterator
	{
	public:
		ConstIterator(T*)
		{
			static_assert(true);
		}
		friend bool operator==(const ConstIterator&, const ConstIterator&)
		{
			static_assert(true);
		}
		friend bool operator!=(const ConstIterator&, const ConstIterator&)
		{
			static_assert(true);
		}
		T& operator*() const
		{
			static_assert(true);
		}
		ConstIterator& operator++()
		{
			static_assert(true);
		}
		ConstIterator operator++(s32)
		{
			static_assert(true);
		}
		T* operator->()
		{
			static_assert(true);
		}
	};

	template<typename T, typename IMPL = void>
	class Iterator
	{
	public:
		Iterator(T*)
		{
			static_assert(true);
		}
		friend bool operator==(const Iterator&, const Iterator&)
		{
			static_assert(true);
		}
		friend bool operator!=(const Iterator&, const Iterator&)
		{
			static_assert(true);
		}
		T& operator*() const
		{
			static_assert(true);
		}
		Iterator& operator++()
		{
			static_assert(true);
		}
		Iterator operator++(s32)
		{
			static_assert(true);
		}
		T* operator->()
		{
			static_assert(true);
		}
	};
	template<typename T, typename IMPL = void>
	struct ReverseIterator
	{

	};
	template<typename T, typename IMPL = void>
	struct ConstReverseIterator
	{

	};
}

#ifdef STL
#include "STL/STL_Iterator.h"
#endif