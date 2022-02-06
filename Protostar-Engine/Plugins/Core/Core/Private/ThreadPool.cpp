#include "../Header Files/Public/ThreadPool.h"

#pragma region Internal Includes 
#pragma endregion //Internal Includes
#pragma region External Includes
#include <functional>
#pragma endregion //External Includes

namespace Protostar
{
	void PThread::WorkerRun()
	{
		std::mutex waitMutex;
		std::unique_lock<std::mutex> waitLock(waitMutex);
		while (!m_stopThread.load())
		{
			m_semaphore.wait(waitLock, [this]()->bool {return m_owner->HasQueuedJob() || m_owner->StopThreads(); });
			while (m_owner->ReserveJob())
			{
				PBaseJob* currentJob = m_owner->DequeueJob();
				currentJob->Execute();
				delete currentJob;
				currentJob = nullptr;
			}
		}
	}

	PThreadPool::PThreadPool(const s32 _threadAmount)
		:m_stopThreads(false)
	{
		m_threads = std::vector<PThread*>(_threadAmount);
		m_threadAmount = _threadAmount;
		for (s32 i = 0; i < _threadAmount; ++i)
		{
			m_threads[i] = std::move(new PThread(i, this, m_semaphore, m_stopThreads));
		}
	}

	PThreadPool::~PThreadPool()
	{
		m_stopThreads.exchange(true);
		m_semaphore.notify_all();

		for (PThread* thread : m_threads)
		{
			delete thread;
		}
		m_threads.clear();
	}

	void PThreadPool::QueueJob(PBaseJob* _jobToQueue)
	{
		m_jobQueue.push(_jobToQueue);
		m_queueCount.fetch_add(1);
		m_semaphore.notify_one();
	}

	bool PThreadPool::HasQueuedJob()
	{
		const std::lock_guard<std::mutex> lock(m_queueLock);
		bool bHasQueuedJob = m_jobQueue.size() > 0;
		return bHasQueuedJob;
	}

	bool PThreadPool::StopThreads()
	{
		return m_stopThreads.load();
	}

	PBaseJob* PThreadPool::DequeueJob()
	{
		const std::lock_guard<std::mutex> lock(m_queueLock);
		PBaseJob* returnJob = m_jobQueue.front();
		m_jobQueue.pop();
		return returnJob;
	}
	bool PThreadPool::ReserveJob()
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