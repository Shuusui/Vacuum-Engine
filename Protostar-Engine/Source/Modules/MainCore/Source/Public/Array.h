#pragma once
#include <cstddef>
#include "GlobalDefinitions.h"
#include "GlobalMacros.h"
#include "Allocator.h"
#include "Iterator.h"

namespace Protostar::Core
{
	template<typename T, typename ALLOCATOR = Allocator<T>, typename IMPL = void>
	class Array 
	{
	public:
		constexpr Array() noexcept(noexcept(ALLOCATOR())) requires false
		{
			
		}
		constexpr explicit Array(const ALLOCATOR& _allocator) noexcept requires false
		{
			
		}
		constexpr Array(std::size_t _count, const T& _value, const ALLOCATOR _allocator = ALLOCATOR()) requires false
		{
			
		}
		constexpr Array(std::size_t _count, const ALLOCATOR& _allocator = ALLOCATOR()) requires false
		{
			
		}
		template<typename INPUT_IT>
		constexpr Array(INPUT_IT _first, INPUT_IT _last, const ALLOCATOR& _allocator = ALLOCATOR()) requires false
		{
			
		}
		constexpr Array(const Array& _other) requires false
		{
			
		}
		constexpr Array(const Array& _other, const ALLOCATOR& _allocator) requires false
		{
			
		}
		constexpr Array(Array&& _other) noexcept requires false
		{
			
		}
		constexpr Array(Array&& _other, const ALLOCATOR& _allocator) requires false
		{
			
		}
		constexpr Array(std::initializer_list<T> _init, const ALLOCATOR& _allocator = ALLOCATOR()) requires false
		{
			
		}
		constexpr Array& operator=(const Array& _other) requires false
		{
			
		}
		constexpr Array& operator=(Array&& _other) noexcept requires false
		{
			
		}
		constexpr Array& operator=(std::initializer_list<T> _list) requires false
		{
			
		}
		constexpr void Replace(std::size_t _amount, const T& _value) requires false
		{
			
		}
		template<typename INPUT_IT>
		constexpr void Replace(INPUT_IT _first, INPUT_IT _last) requires false
		{
			
		}
		constexpr void Replace(std::initializer_list<T> _list) requires false
		{
			
		}
		PE_NODISCARD constexpr ALLOCATOR GetAllocator() const noexcept requires false
		{
		}
		PE_NODISCARD constexpr T& At(std::size_t _pos) requires false
		{
			
		}
		PE_NODISCARD constexpr const T& At(std::size_t _pos) const requires false
		{
			
		}
		PE_NODISCARD constexpr T& operator[](std::size_t _pos) requires false
		{
			
		}
		PE_NODISCARD constexpr const T& operator[](std::size_t _pos) const requires false
		{
			
		}
		PE_NODISCARD constexpr T& GetFirst() requires false
		{
			
		}
		PE_NODISCARD constexpr const T& GetFirst() const requires false
		{
			
		}
		PE_NODISCARD constexpr T& GetLast() requires false
		{
			
		}
		PE_NODISCARD constexpr const T& GetLast() const requires false
		{
			
		}
		PE_NODISCARD constexpr T* GetData() noexcept requires false
		{
			
		}
		PE_NODISCARD constexpr const T* GetData() const noexcept requires false
		{
			
		}
		PE_NODISCARD constexpr bool IsEmpty() const noexcept requires false
		{
			
		}
		PE_NODISCARD constexpr std::size_t GetSize() const noexcept requires false
		{
			
		}
		PE_NODISCARD constexpr std::size_t GetMaxSize() const noexcept requires false
		{
			
		}
		PE_NODISCARD constexpr void Reserve(std::size_t _newCapacity) requires false
		{
			
		}
		PE_NODISCARD constexpr std::size_t GetCapacity() const noexcept requires false
		{
			
		}
		constexpr void Shrink() requires false
		{
			
		}
		constexpr void Clear() noexcept requires false
		{
			
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, const T& _value) requires false
		{
			
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, T&& _value) requires false
		{
			
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, std::size_t _count, const T& _value) requires false
		{
			
		}
		template<typename INPUT_IT>
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, INPUT_IT _first, INPUT_IT _last) requires false
		{
			
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, std::initializer_list<T> _list) requires false
		{
			
		}
		template<typename... ARGS>
		constexpr Iterator<T> EmplaceAt(ConstIterator<T> _pos, ARGS&&... _args) requires false
		{
			
		}
		template<typename... ARGS>
		constexpr T& Emplace(ARGS&&... _args) requires false
		{
			
		}
		constexpr Iterator<T> Remove(ConstIterator<T> _pos) requires false
		{
			
		}
		constexpr Iterator<T> Remove(ConstIterator<T> _first, ConstIterator<T> _last) requires false
		{
			
		}
		constexpr void Add(const T& _value) requires false
		{
			
		}
		constexpr void Add(T&& _value) requires false 
		{
			
		}
		constexpr void RemoveLast() requires false
		{
			
		}
		constexpr void Resize(std::size_t _count) requires false
		{
			
		}
		constexpr void Resize(std::size_t _count, const T& _value) requires false
		{
			
		}
		constexpr void Swap(Array& _other) noexcept requires false
		{
			
		}
		constexpr Iterator<T> begin() noexcept requires false
		{
			
		}
		constexpr ConstIterator<T> begin() const noexcept requires false
		{
			
		}
		constexpr ConstIterator<T> cbegin() const noexcept requires false
		{
			
		}
		constexpr Iterator<T> end() noexcept requires false
		{
			
		}
		constexpr ConstIterator<T> end() const noexcept requires false
		{
			
		}
		constexpr ConstIterator<T> cend() const noexcept requires false
		{
			
		}
		constexpr ReverseIterator<T> rbegin() noexcept requires false
		{
			
		}
		constexpr ConstReverseIterator<T> rbegin() const noexcept requires false
		{
			
		}
		constexpr ConstReverseIterator<T> crbegin() const noexcept requires false
		{
			
		}
		constexpr ReverseIterator<T> rend() noexcept requires false
		{
			
		}
		constexpr ConstReverseIterator<T> rend() const noexcept requires false
		{
			
		}
		constexpr ConstReverseIterator<T> crend() const noexcept requires false
		{
			
		}
	};

	template<typename T, std::size_t SIZE, typename IMPL = void>
	class FixedSizeArray
	{
	public:
		PE_NODISCARD constexpr T& At(std::size_t)
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr const T& At(std::size_t) const 
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr T& operator[](std::size_t)
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr const T& operator[](std::size_t) const
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr T& GetFirst()
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr const T& GetFirst() const
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr T& GetLast()
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr const T& GetLast() const
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr T* GetData() noexcept
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr const T* GetData() const noexcept
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr bool IsEmpty() const noexcept
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr std::size_t GetSize() const noexcept
		{
			static_assert(true);
		}
		PE_NODISCARD constexpr std::size_t GetMaxSize() const noexcept
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