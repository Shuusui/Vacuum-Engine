#include "ThreadPool.h"

#include <functional>

namespace Protostar::Core
{
	void Thread::WorkerRun()
	{
		std::mutex waitMutex;
		std::unique_lock<std::mutex> waitLock(waitMutex);
		while (!m_stopThread.load())
		{
			m_semaphore.wait(waitLock, [this]()->bool {return m_owner->HasQueuedJob() || m_owner->StopThreads(); });
			while (m_owner->ReserveJob())
			{
				BaseJob* currentJob = m_owner->DequeueJob();
				currentJob->Execute();
				delete currentJob;
				currentJob = nullptr;
			}
		}
	}

	ThreadPool::ThreadPool(const s32 _threadAmount)
		:m_stopThreads(false)
	{
		m_threads = std::vector<Thread*>(_threadAmount);
		m_threadAmount = _threadAmount;
		for (s32 i = 0; i < _threadAmount; ++i)
		{
			m_threads[i] = std::move(new Thread(i, this, m_semaphore, m_stopThreads));
		}
	}

	ThreadPool::~ThreadPool()
	{
		m_stopThreads.exchange(true);
		m_semaphore.notify_all();

		for (Thread* thread : m_threads)
		{
			delete thread;
		}
		m_threads.clear();
	}

	void ThreadPool::QueueJob(BaseJob* _jobToQueue)
	{
		m_jobQueue.push(_jobToQueue);
		m_queueCount.fetch_add(1);
		m_semaphore.notify_one();
	}

	bool ThreadPool::HasQueuedJob()
	{
		const std::lock_guard<std::mutex> lock(m_queueLock);
		bool bHasQueuedJob = m_jobQueue.size() > 0;
		return bHasQueuedJob;
	}

	bool ThreadPool::StopThreads()
	{
		return m_stopThreads.load();
	}

	BaseJob* ThreadPool::DequeueJob()
	{
		const std::lock_guard<std::mutex> lock(m_queueLock);
		BaseJob* returnJob = m_jobQueue.front();
		m_jobQueue.pop();
		return returnJob;
	}
	bool ThreadPool::ReserveJob()
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