#pragma once
#include <cstddef>
#include <initializer_list>
#include "GlobalDefinitions.h"
#include "Allocator.h"
#include "Iterator.h"

namespace Protostar::Core
{
	template<typename T, typename ALLOCATOR = Allocator<T>, typename IMPL = void>
	class Array
	{
	public:
		constexpr Array() noexcept(noexcept(ALLOCATOR()))
		{
			static_assert(true);
		}
		constexpr explicit Array(const ALLOCATOR& _allocator) noexcept
		{
			static_assert(true);
		}
		constexpr Array(std::size_t _count, const T& _value, const ALLOCATOR _allocator = ALLOCATOR())
		{
			static_assert(true);
		}
		constexpr Array(std::size_t _count, const ALLOCATOR& _allocator = ALLOCATOR())
		{
			static_assert(true);
		}
		template<typename INPUTIT>
		constexpr Array(INPUTIT _first, INPUTIT _last, const ALLOCATOR& _allocator = ALLOCATOR())
		{
			static_assert(true);
		}
		constexpr Array(const Array& _other)
		{
			static_assert(true);
		}
		constexpr Array(const Array& _other, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		constexpr Array(Array&& _other) noexcept
		{
			static_assert(true);
		}
		constexpr Array(Array&& _other, const ALLOCATOR& _allocator)
		{
			static_assert(true);
		}
		constexpr Array(std::initializer_list<T> _init, const ALLOCATOR& _allocator = ALLOCATOR())
		{
			static_assert(true);
		}
		constexpr Array& operator=(const Array& _other)
		{
			static_assert(true);
		}
		constexpr Array& operator=(Array&& _other) noexcept
		{
			static_assert(true);
		}
		constexpr Array& operator=(std::initializer_list<T> _list)
		{
			static_assert(true);
		}
		constexpr void Replace(std::size_t _amount, const T& _value)
		{
			static_assert(true);
		}
		template<typename INPUTIT>
		constexpr void Replace(INPUTIT _first, INPUTIT _last)
		{
			static_assert(true);
		}
		constexpr void Replace(std::initializer_list<T> _list)
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr ALLOCATOR GetAllocator() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T& At(std::size_t _pos)
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& At(std::size_t _pos) const
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T& operator[](std::size_t _pos)
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& operator[](std::size_t _pos) const
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T& GetFirst()
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& GetFirst() const
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T& GetLast()
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& GetLast() const
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T* GetData() noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T* GetData() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr bool IsEmpty() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr std::size_t GetSize() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr std::size_t GetMaxSize() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr void Reserve(std::size_t _newCapacity)
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr std::size_t GetCapacity() const noexcept
		{
			static_assert(true);
		}
		constexpr void Shrink()
		{
			static_assert(true);
		}
		constexpr void Clear() noexcept
		{
			static_assert(true);
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, const T& _value)
		{
			static_assert(true);
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, T&& _value)
		{
			static_assert(true);
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, std::size_t _count, const T& _value)
		{
			static_assert(true);
		}
		template<typename INPUTIT>
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, INPUTIT _first, INPUTIT _last)
		{
			static_assert(true);
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, std::initializer_list<T> _list)
		{
			static_assert(true);
		}
		template<typename... ARGS>
		constexpr Iterator<T> EmplaceAt(ConstIterator<T> _pos, ARGS&&... _args)
		{
			static_assert(true);
		}
		template<typename... ARGS>
		constexpr T& Emplace(ARGS&&... _args)
		{
			static_assert(true);
		}
		constexpr Iterator<T> Remove(ConstIterator<T> _pos)
		{
			static_assert(true);
		}
		constexpr Iterator<T> Remove(ConstIterator<T> _first, ConstIterator<T> _last)
		{
			static_assert(true);
		}
		constexpr void Add(const T& _value)
		{
			static_assert(true);
		}
		constexpr void Add(T&& _value)
		{
			static_assert(true);
		}
		constexpr void RemoveLast()
		{
			static_assert(true);
		}
		constexpr void Resize(std::size_t _count)
		{
			static_assert(true);
		}
		constexpr void Resize(std::size_t _count, const T& _value)
		{
			static_assert(true);
		}
		constexpr void Swap(Array& _other) noexcept
		{
			static_assert(true);
		}
		constexpr Iterator<T> begin() noexcept
		{
			static_assert(true);
		}
		constexpr ConstIterator<T> begin() const noexcept
		{
			static_assert(true);
		}
		constexpr ConstIterator<T> cbegin() const noexcept
		{
			static_assert(true);
		}
		constexpr Iterator<T> end() noexcept
		{
			static_assert(true);
		}
		constexpr ConstIterator<T> end() const noexcept
		{
			static_assert(true);
		}
		constexpr ConstIterator<T> cend() const noexcept
		{
			static_assert(true);
		}
		constexpr ReverseIterator<T> rbegin() noexcept
		{
			static_assert(true);
		}
		constexpr ConstReverseIterator<T> rbegin() const noexcept
		{
			static_assert(true);
		}
		constexpr ConstReverseIterator<T> crbegin() const noexcept
		{
			static_assert(true);
		}
		constexpr ReverseIterator<T> rend() noexcept
		{
			static_assert(true);
		}
		constexpr ConstReverseIterator<T> rend() const noexcept
		{
			static_assert(true);
		}
		constexpr ConstReverseIterator<T> crend() const noexcept
		{
			static_assert(true);
		}
	};

	template<typename T, std::size_t SIZE, typename IMPL = void>
	class FixedSizeArray
	{
	public:
		[[nodiscard]] constexpr T& At(std::size_t)
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& At(std::size_t) const 
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T& operator[](std::size_t)
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& operator[](std::size_t) const
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T& GetFirst()
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& GetFirst() const
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T& GetLast()
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T& GetLast() const
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr T* GetData() noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr const T* GetData() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr bool IsEmpty() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr std::size_t GetSize() const noexcept
		{
			static_assert(true);
		}
		[[nodiscard]] constexpr std::size_t GetMaxSize() const noexcept
		{
			static_assert(true);
		}
		constexpr void Fill(const T& _value)
		{
			static_assert(true);
		}
		constexpr void Swap(FixedSizeArray& _other) noexcept
		{
			static_assert(true);
		}
	};
}

#ifdef STL
#include "STL/STL_Array.h"
#include "STL/STL_FixedSizeArray.h"
#endif