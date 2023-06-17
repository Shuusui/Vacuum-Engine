#pragma once

#ifdef STL 
namespace Protostar::Core
{
	template<typename T>
	class WeakPtr
	{
	public:
		T* operator->()
		{
			return m_ptr;
		}
		
		~WeakPtr()
		{
			if (m_ptr)
			{
				m_owner.RemoveWeakPtr(m_index);
			}
		}

		WeakPtr<T>& operator=(std::nullptr_t) noexcept
		{
			m_index = -1;
			m_ptr = nullptr;
		}

		WeakPtr<T>& operator=(WeakPtr<T>&& _right) noexcept
		{
			m_index = _right.m_index;
			m_ptr = _right.m_ptr;
		}
	private:
		template<typename T>
		friend class UniquePtr;

		WeakPtr(UniquePtr<T>& _owner)
			:m_index(std::size_t(-1))
			,m_ptr(nullptr)
			,m_owner(_owner)
		{
		}

		WeakPtr(std::size_t _index, T* _ptr, UniquePtr<T>& _owner)
			:m_index(_index)
			,m_ptr(_ptr)
			,m_owner(_owner)
		{
		}

		void Cleanup()
		{
			m_ptr = nullptr;
		}

		std::size_t m_index;
		T* m_ptr;
		UniquePtr<T>& m_owner;
	};
}
#endif