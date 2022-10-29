#include <iterator>
#include <cstddef>
#include <vector>

namespace Protostar::Core
{
	template<typename T> 
	class ConstIterator<T, void>
	{
	public:
		using iterator_category = std::contiguous_iterator_tag;
		using differene_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;

		ConstIterator(std::vector<T>::const_iterator _iterator)
			:m_ptr(_iterator._Ptr)
		{
		}

		ConstIterator(pointer _ptr)
			:m_ptr(_ptr)
		{
		}

		friend bool operator==(const ConstIterator& _lhs, const ConstIterator& _rhs)
		{
			return _lhs.m_ptr == _rhs.m_ptr;
		}

		friend bool operator!=(const ConstIterator& _lhs, const ConstIterator& _rhs)
		{
			return _lhs.m_ptr != _rhs.m_ptr;
		}

		reference operator*() const
		{
			return *m_ptr;
		}

		pointer operator->()
		{
			return m_ptr;
		}

		ConstIterator& operator++()
		{
			m_ptr++;
			return *this;
		}

		ConstIterator operator++(s32)
		{
			ConstIterator tmp = *this;
			++(*this); 
			return tmp;
		}

	private:
		pointer m_ptr;
	};

	template<typename T>
	class Iterator<T, void>
	{
	public:
		using iterator_category = std::contiguous_iterator_tag;
		using differene_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		Iterator(std::vector<T>::iterator _iterator)
			:m_ptr(_iterator._Ptr)
		{}
		Iterator(pointer _ptr)
			:m_ptr(_ptr)
		{}

		friend bool operator==(const Iterator& _lhs, const Iterator& _rhs)
		{
			return _lhs.m_ptr == _rhs.m_ptr;
		}

		friend bool operator!=(const Iterator& _lhs, const Iterator& _rhs)
		{
			return _lhs.m_ptr != _rhs.m_ptr;
		}

		reference operator*() const
		{
			return *m_ptr;
		}

		pointer operator->()
		{
			return m_ptr;
		}

		Iterator& operator++()
		{
			m_ptr++;
			return *this;
		}

		Iterator operator++(s32)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

	private:
		pointer m_ptr;
	};
}