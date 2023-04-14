#pragma once
#include "Allocator.h"
#include "Hash.h"
#include "Pair.h"
#include <xstddef>

namespace Protostar::Core
{
	template<typename KEY, typename HASH = Hash<KEY>, typename KEY_EQUAL = std::equal_to<KEY>, typename ALLOCATOR = Allocator<KEY>, typename IMPL = void>
	class HashSet
	{
	public:
		HashSet(std::size_t _bucketCount, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		HashSet(std::size_t _bucketCount, const HASH& _hash, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		explicit HashSet(const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		template<typename INPUT_IT>
		HashSet(INPUT_IT _first, INPUT_IT _last,
			std::size_t _bucketCount = 0, const HASH& _hash = HASH(), const KEY_EQUAL& _equal = KEY_EQUAL(), const ALLOCATOR& _allocator = Allocator())
		{
			static_assert(true);
		}
		template<typename INPUT_IT>
		HashSet(INPUT_IT _first, INPUT_IT _last, std::size_t _bucketCount, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		template<typename INPUT_IT> 
		HashSet(INPUT_IT _first, INPUT_IT _last, std::size_t _bucketCount, const HASH& _hash, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		HashSet(const HashSet& _other)
		{
			static_assert(true);
		}
		HashSet(const HashSet& _other, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		HashSet(HashSet&& _other)
		{
			static_assert(true);
		}
		HashSet(HashSet&& _other, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		HashSet(std::initializer_list<KEY> _initializer, std::size_t _bucketCount = 0,
			const HASH& _hash = HASH(), const KEY_EQUAL& _equal = KEY_EQUAL(), 
			const ALLOCATOR& _allocator = Allocator())
		{
			static_assert(true);
		}
		HashSet(std::initializer_list<KEY> _initializer, std::size_t _bucketCount, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		HashSet(std::initializer_list<KEY> _initializer, std::size_t _bucketCount, const HASH& _hash, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		~HashSet()
		{
			static_assert(true);
		}
		HashSet& operator=(const HashSet& _other)
		{
			static_assert(true);
		}
		HashSet& operator=(HashSet& _other) noexcept
		{
			static_assert(true);
		}
		HashSet& operator=(std::initializer_list<KEY> _initializerList)
		{
			static_assert(true);
		}
		Allocator GetAllocator() const noexcept
		{
			static_assert(true);
		}
		Iterator<KEY> begin() noexcept
		{
			static_assert(true);
		}
		ConstIterator<KEY> begin() const noexcept
		{
			static_assert(true);
		}
		ConstIterator<KEY> cbegin() const noexcept
		{
			static_assert(true);
		}
		Iterator<KEY> end() noexcept
		{
			static_assert(true);
		}
		ConstIterator<KEY> end() const noexcept
		{
			static_assert(true);
		}
		ConstIterator<KEY> cend() const noexcept
		{
			static_assert(true);
		}
		PE_NODISCARD bool IsEmpty() const noexcept 
		{
			static_assert(true);
		}
		std::size_t GetSize() const noexcept
		{
			static_assert(true);
		}
		std::size_t GetMaxSize() const noexcept
		{
			static_assert(true);
		}
		void Clear() noexcept
		{
			static_assert(true);
		}
		Pair<Iterator, bool> Insert(const KEY& _value)
		{
			static_assert(true);
		}
		Pair<Iterator, bool> Insert(KEY&& _value)
		{
			static_assert(true);
		}
		Iterator Insert(ConstIterator _hint, const KEY& _value)
		{
			static_assert(true);
		}
		Iterator Insert(ConstIterator _hint, KEY&& _value)
		{
			static_assert(true);
		}
		template<typename INPUT_IT>
		void Insert(INPUT_IT _first, INPUT_IT _last)
		{
			static_assert(true);
		}
		void Insert(std::initializer_list<KEY> _initializerList)
		{
			static_assert(true);
		}
	};
}