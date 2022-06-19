#pragma once

#include <queue>

namespace Protostar
{
	/**
		* A n-1 thread safe queue (n contributor, 1 substractor)
		*/
	template<typename T>
	class PNOQueue
	{
	public:
		PNOQueue()
			: m_queue()
		{
		}

		PNOQueue(const PNOQueue& _other)
			: m_queue(_other.m_queue)
		{
		}

		PNOQueue(PNOQueue&& _other)
			: m_queue(std::move(_other.m_queue))
		{
		}

		T& Dequeue()
		{
			T& returnValue = m_queue.back();
			m_queue.pop();
			return returnValue;
		}

		void Queue(const T& _value)
		{
			m_queue.push(_value);
		}

		void operator=(const T& _other)
		{
			m_queue = _other.m_queue;
		}

		void operator=(T&& _other)
		{
			m_queue = std::move(_other.m_queue);
		}
	private:
		std::queue<T> m_queue;
	};
}

