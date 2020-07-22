#include "../HeaderFiles/ThreadPool.h"

#pragma region Internal Includes 
#pragma endregion //Internal Includes
#pragma region External Includes
#include <functional>
#pragma endregion //External Includes

namespace Vacuum
{
	namespace Core
	{
		void CThread::WorkerRun()
		{
			while (true)
			{
				if (m_currentJob)
				{
					m_currentJob->Execute();
				}
			}
		}

		CThreadPool::CThreadPool(const int32& _threadAmount)
		{
			m_threads.reserve(_threadAmount);
			m_threadAmount = _threadAmount;
			for (size_t i = 0; i < _threadAmount; ++i)
			{
				m_threads[i] = std::move(CThread(i, this));
				m_freeThreadIndices.push(i);
			}
		}
	}
}