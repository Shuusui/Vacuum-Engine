#pragma once

#include "..\Private\BaseJob.h"
#include "GlobalDefs.h"
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <queue>
#include <unordered_map>
#include <condition_variable>
#include <mutex>

namespace Vacuum
{
	namespace Core
	{
		template<typename T>
		class CJob : public CBaseJob
		{
		public:
			CJob(std::packaged_task<T()>& _task)
				:m_task(std::move(_task))
			{
			}

			virtual void Execute() override
			{
				m_task();
			}
		private:
			std::packaged_task<T()> m_task;
		};

		class CThread
		{
		public:
			/**
			 * Deleted cause it's unnecessary and maybe gets accidentally called
			 */
			CThread() = default;

			/**
			 * The only constructor needed to construct the object
			 * @param _threadIndex The index of the thread in the owners container
			 * @param _owner The owner of this thread. In this case the CThreadPool object
			 */
			CThread(const int32& _threadIndex, class CThreadPool* _owner, std::condition_variable& _semaphore, std::atomic_bool& _stopThread)
				:m_thread(std::bind(&CThread::WorkerRun, this))
				,m_threadIndex(_threadIndex)
				,m_owner(_owner)
				,m_currentJob(nullptr)
				,m_semaphore(_semaphore)
				,m_stopThread(_stopThread)
			{
			}

			/**
			 * move constructor
			 * @param _other The object which should get moved into this
			 */
			CThread(CThread&& _other) noexcept
				:m_thread(std::move(_other.m_thread))
				,m_threadIndex(std::move(_other.m_threadIndex))
				,m_owner(std::move(_other.m_owner))
				,m_currentJob(std::move(_other.m_currentJob))
				,m_semaphore(_other.m_semaphore)
				,m_stopThread(_other.m_stopThread)
			{
				_other.m_threadIndex = -1;
				_other.m_owner = nullptr;
				_other.m_currentJob = nullptr;
			}

			/**
			 * Destructor which joins the thread to the overlying thread if it's joinable. 
			 */
			~CThread()
			{
				if (m_thread.joinable())
				{
					m_thread.join();
				}
			}


		private:
			void WorkerRun();

			std::thread m_thread;
			int32 m_threadIndex;
			class CThreadPool* m_owner;
			CBaseJob* m_currentJob;
			std::condition_variable& m_semaphore;
			std::atomic_bool& m_stopThread;
		};

		class CThreadPool
		{
		public:
			CThreadPool() = delete;
			CThreadPool(const int32& _threadAmount);
			~CThreadPool();
			void QueueJob(CBaseJob* _jobToQueue);
			CBaseJob* DequeueJob();
			bool HasQueuedJob();
			bool ReserveJob();
			bool StopThreads();
		private:
			std::vector<CThread*> m_threads;
			int32 m_threadAmount;
			std::queue<CBaseJob*> m_jobQueue;
			std::mutex m_queueLock;
			std::condition_variable m_semaphore;
			std::atomic_bool m_stopThreads;
			std::atomic_int32_t m_queueCount;
		};
	}
}