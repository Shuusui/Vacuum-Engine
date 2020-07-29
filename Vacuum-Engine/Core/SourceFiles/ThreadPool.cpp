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
			std::mutex waitMutex;
			std::unique_lock<std::mutex> waitLock(waitMutex);
			while (!m_stopThread.load())
			{
				m_semaphore.wait(waitLock, [this]()->bool{return m_owner->HasQueuedJob() || m_owner->StopThreads();});
				while (m_owner->ReserveJob())
				{
					CBaseJob* currentJob = m_owner->DequeueJob();
					currentJob->Execute();
					delete currentJob;
					currentJob = nullptr;
				}
			}
		}

		CThreadPool::CThreadPool(const int32& _threadAmount)
		{
			m_threads = std::vector<CThread*>(_threadAmount);
			m_threadAmount = _threadAmount;
			for (size_t i = 0; i < _threadAmount; ++i)
			{
				m_threads[i] = std::move(new CThread(i, this, m_semaphore, m_stopThreads));
			}
		}

		CThreadPool::~CThreadPool()
		{
			m_stopThreads.exchange(true);
			m_semaphore.notify_all();

			for (CThread* thread : m_threads)
			{
				delete thread;
			}
			m_threads.clear();
		}

		void CThreadPool::QueueJob(CBaseJob* _jobToQueue)
		{
			m_jobQueue.push(_jobToQueue);
			m_queueCount.fetch_add(1);
			m_semaphore.notify_one();
		}

		bool CThreadPool::HasQueuedJob()
		{
			const std::lock_guard<std::mutex> lock(m_queueLock);
			bool bHasQueuedJob = m_jobQueue.size() > 0;
			return bHasQueuedJob;
		}

		bool CThreadPool::StopThreads()
		{
			return m_stopThreads.load();
		}

		CBaseJob* CThreadPool::DequeueJob()
		{
			const std::lock_guard<std::mutex> lock(m_queueLock);
			CBaseJob* returnJob = m_jobQueue.back();
			m_jobQueue.pop();
			return returnJob;
		}
		bool CThreadPool::ReserveJob()
		{
			bool bReturn = false;
			const std::lock_guard<std::mutex> lock(m_queueLock);
			if (m_queueCount.load() > 0)
			{
				bReturn = true;
				m_queueCount.fetch_sub(1);
			}
			return bReturn;
		}
	}
}