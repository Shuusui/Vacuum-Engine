#include "../HeaderFiles/Public/ThreadPool.h"

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
				m_currentJob =  m_owner->DequeueJob();
				m_currentJob->Execute();
				delete m_currentJob;
				m_currentJob = nullptr;
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

		void CThreadPool::QueueJob(CBaseJob* _jobToQueue)
		{
			m_jobQueue.push(_jobToQueue);
		}

		CBaseJob* CThreadPool::DequeueJob()
		{
			std::unique_lock<std::mutex> uniqueLock(m_queueLock, std::defer_lock);
			uniqueLock.lock();
			CBaseJob* returnJob = m_jobQueue.back();
			m_jobQueue.pop();
			uniqueLock.unlock();
			return returnJob;
		}
	}
}