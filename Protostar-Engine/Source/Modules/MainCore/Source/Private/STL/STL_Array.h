#include <vector>
#include <cstddef>
#include "Iterator.h"

namespace Protostar::Core
{
	template<typename T, typename ALLOCATOR>
	class Array<T, ALLOCATOR, void>
	{
	public:
		constexpr Array() noexcept(noexcept(ALLOCATOR()))
			:m_container()
		{
		}
		constexpr explicit Array(const ALLOCATOR& _allocator) noexcept
			:m_container(_allocator)
		{
		}
		constexpr Array(std::size_t _count, const T& _value, const ALLOCATOR _allocator = ALLOCATOR())
			:m_container(_count, _value, _allocator)
		{
		}
		constexpr Array(std::size_t _count, const ALLOCATOR& _allocator = ALLOCATOR())
			:m_container(_count, _allocator)
		{
		}
		template<typename INPUTIT>
		constexpr Array(INPUTIT _first, INPUTIT _last, const ALLOCATOR& _allocator = ALLOCATOR())
			:m_container(_first, _last, _allocator)
		{
		}
		constexpr Array(const Array& _other)
			:m_container(_other.m_container)
		{
		}
		constexpr Array(const Array& _other, const ALLOCATOR& _allocator)
			:m_container(_other.m_container, _allocator)
		{
		}
		constexpr Array(Array&& _other) noexcept
			:m_container(_other.m_container)
		{
		}
		constexpr Array(Array&& _other, const ALLOCATOR& _allocator)
			:m_container(_other.m_container, _allocator)
		{
		}
		constexpr Array(std::initializer_list<T> _init, const ALLOCATOR& _allocator = ALLOCATOR())
			:m_container(_init, _allocator)
		{
		}
		constexpr Array& operator=(const Array& _other)
		{
			m_container = _other.m_container;
			return *this;
		}
		constexpr Array& operator=(Array&& _other) noexcept
		{
			m_container = _other.m_container;
			return *this;
		}
		constexpr Array& operator=(std::initializer_list<T> _list)
		{
			m_container = _list;
			return *this;
		}
		constexpr void Replace(std::size_t _amount, const T& _value)
		{
			m_container.assign(_amount, _value);
		}
		template<typename INPUTIT>
		constexpr void Replace(INPUTIT _first, INPUTIT _last)
		{
			m_container.assign(_first, _last);
		}
		constexpr void Replace(std::initializer_list<T> _list)
		{
			m_container.assign(_list);
		}
		[[nodiscard]] constexpr ALLOCATOR GetAllocator() const noexcept
		{
			return m_container.get_allocator();
		}
		[[nodiscard]] constexpr T& At(std::size_t _pos)
		{
			return m_container.at(_pos);
		}
		[[nodiscard]] constexpr const T& At(std::size_t _pos) const
		{
			return m_container.at(_pos);
		}
		[[nodiscard]] constexpr T& operator[](std::size_t _pos)
		{
			return m_container[_pos];
		}
		[[nodiscard]] constexpr const T& operator[](std::size_t _pos) const
		{
			return m_container[_pos];
		}
		[[nodiscard]] constexpr T& GetFirst()
		{
			return m_container.front();
		}
		[[nodiscard]] constexpr const T& GetFirst() const
		{
			return m_container.front();
		}
		[[nodiscard]] constexpr T& GetLast()
		{
			return m_container.back();
		}
		[[nodiscard]] constexpr const T& GetLast() const
		{
			return m_container.back();
		}
		[[nodiscard]] constexpr T* GetData() noexcept
		{
			return m_container.data();
		}
		[[nodiscard]] constexpr const T* GetData() const noexcept
		{
			return m_container.data();
		}
		[[nodiscard]] constexpr bool IsEmpty() const noexcept
		{
			return m_container.empty();
		}
		[[nodiscard]] constexpr std::size_t GetSize() const noexcept
		{
			return m_container.size();
		}
		[[nodiscard]] constexpr std::size_t GetMaxSize() const noexcept
		{
			return m_container.max_size();
		}
		[[nodiscard]] constexpr void Reserve(std::size_t _newCapacity)
		{
			m_container.reserve(_newCapacity);
		}
		[[nodiscard]] constexpr std::size_t GetCapacity() const noexcept
		{
			return m_container.capacity();
		}
		constexpr void Shrink()
		{
			m_container.shrink_to_fit();
		}
		constexpr void Clear() noexcept
		{
			m_container.clear();
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, const T& _value)
		{
			return m_container.insert(_pos, _value);
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, T&& _value)
		{
			return m_container.insert(_pos, _value);
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, std::size_t _count, const T& _value)
		{
			return m_container.insert(_pos, _count, _value);
		}
		template<typename INPUTIT>
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, INPUTIT _first, INPUTIT _last)
		{
			return m_container.insert(_pos, _first, _last);
		}
		constexpr Iterator<T> Insert(ConstIterator<T> _pos, std::initializer_list<T> _list)
		{
			return m_container.insert(_pos, _list);
		}
		template<typename... ARGS>
		constexpr Iterator<T> EmplaceAt(ConstIterator<T> _pos, ARGS&&... _args)
		{
			return m_container.emplace(_pos, _args);
		}
		template<typename... ARGS>
		constexpr T& Emplace(ARGS&&... _args)
		{
			return m_container.emplace_back(_args);
		}
		constexpr Iterator<T> Remove(ConstIterator<T> _pos)
		{
			return m_container.erase(_pos);
		}
		constexpr Iterator<T> Remove(ConstIterator<T> _first, ConstIterator<T> _last)
		{
			return m_container.erase(_first, _last);
		}
		constexpr void Add(const T& _value)
		{
			m_container.push_back(_value);
		}
		constexpr void Add(T&& _value)
		{
			m_container.push_back(_value);
		}
		constexpr void RemoveLast()
		{
			m_container.pop_back();
		}
		constexpr void Resize(std::size_t _count)
		{
			m_container.resize(_count);
		}
		constexpr void Resize(std::size_t _count, const T& _value)
		{
			m_container.resize(_count);
		}
		constexpr void Swap(Array& _other) noexcept
		{
			m_container.swap(_other.m_container);
		}
		constexpr Iterator<T> begin() noexcept
		{
			return m_container.begin();
		}
		constexpr ConstIterator<T> begin() const noexcept
		{
			return m_container.begin();
		}
		constexpr ConstIterator<T> cbegin() const noexcept
		{
			return m_container.cbegin();
		}
		constexpr Iterator<T> end() noexcept
		{
			return m_container.end();
		}
		constexpr ConstIterator<T> end() const noexcept
		{
			return m_container.end();
		}
		constexpr ConstIterator<T> cend() const noexcept
		{
			return m_container.cend();
		}
		constexpr ReverseIterator<T> rbegin() noexcept
		{
			return m_container.rbegin();
		}
		constexpr ConstReverseIterator<T> rbegin() const noexcept
		{
			return m_container.rbegin();
		}
		constexpr ConstReverseIterator<T> crbegin() const noexcept
		{
			return m_container.crbegin();
		}
		constexpr ReverseIterator<T> rend() noexcept
		{
			return m_container.rend();
		}
		constexpr ConstReverseIterator<T> rend() const noexcept
		{
			return m_container.rend();
		}
		constexpr ConstReverseIterator<T> crend() const noexcept
		{
			return m_container.crend();
		}
	private:
		std::vector<T> m_container;
	};
}
