#pragma once

#ifdef STL
#include <memory>
#include "Array.h"
#include "WeakPtr.h"

namespace Protostar::Core
{
	template<typename T>
	class UniquePtr
	{
	public:
		UniquePtr()
			:m_uniquePtr(nullptr)
		{
		}

		UniquePtr(T* _ptr)
			:m_uniquePtr(_ptr)
		{
		}

		WeakPtr<T> GetWeakPtr() const
		{
			WeakPtr<T> _weakPtr = WeakPtr<T>(m_weakPtrReferences.GetSize(), m_uniquePtr.get(), *this);
			m_weakPtrReferences.Add(_weakPtr);
			return _weakPtr;
		}

		~UniquePtr()
		{
			Release();
		}

		T* Release()
		{
			T* pointer = m_uniquePtr.release();
			for (WeakPtr<T>& weakPtrReference : m_weakPtrReferences)
			{
				weakPtrReference.Cleanup();
			}
			m_weakPtrReferences.Clear();
			return pointer;
		}

		T* Get() const noexcept
		{
			return m_uniquePtr.get();
		}

		operator bool() const
		{
			return !!m_uniquePtr;
		}

		UniquePtr<T>& operator=(std::unique_ptr<T>&& _stdUniquePtr)
		{
			m_uniquePtr = std::move(_stdUniquePtr);
			return *this;
		}

		T* operator->()
		{
			return m_uniquePtr.get();
		}

		UniquePtr<T>& operator=(std::nullptr_t) noexcept
		{
			m_uniquePtr.reset();
			return *this;
		}

		UniquePtr<T>& operator=(UniquePtr<T>&& _right) noexcept
		{
			m_uniquePtr = std::move(_right.m_uniquePtr);
		}

	private:
		template<typename U>
		friend class WeakPtr;

		void RemoveWeakPtr(std::size_t _index)
		{
			m_weakPtrReferences[_index] = WeakPtr<T>(*this);
		}

		std::unique_ptr<T> m_uniquePtr;
		Array<WeakPtr<T>> m_weakPtrReferences;
	};
}

#endif

