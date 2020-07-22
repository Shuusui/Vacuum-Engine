#pragma once

#include "GlobalDefs.h"
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <queue>
#include <unordered_map>
#include <condition_variable>

namespace Vacuum
{
	namespace Core
	{
		/**
		 * Don't use this class directly, please use derived classes to create jobs. This is just the base class for the queue.
		 * You can override this class with the execute function to make more implementations possible.
		 */
		class CJob
		{
		public:
			CJob() = delete;
			virtual ~CJob() {};
			virtual void Execute() = 0;
		private:
		};

		template<typename T>
		class CReturnJob : public CJob
		{
		public:
			CReturnJob(std::packaged_task<T()>& _task)
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

		template<typename T>
		class CValJob : public CJob
		{
		public:
			CValJob(std::packaged_task<void(T)>& _task)
				:m_task(_task)
			{
			}
			virtual void Execute() override
			{
				m_task();
			}
		private:
			std::packaged_task<void(T)> m_task;
		};

		class CThread
		{
		public:
			CThread(const int32& _threadIndex, class CThreadPool* _owner)
				:m_thread(std::bind(&CThread::WorkerRun, this))
				,m_threadIndex(_threadIndex)
				,m_owner(_owner)
			{
			}

			CThread(CThread&& _other) noexcept
				:m_thread(std::move(_other.m_thread))
				,m_threadIndex(std::move(_other.m_threadIndex))
				,m_owner(std::move(_other.m_owner))
			{
			}

			CThread& operator=(CThread&& _other) noexcept
			{
				return *this;
			}

			~CThread()
			{
				if (m_thread.joinable())
				{
					m_thread.join();
				}
			}

			void WorkerRun();

		private:
			std::thread m_thread;
			int32 m_threadIndex;
			class CThreadPool* m_owner;
			CJob* m_currentJob;
			std::condition_variable m_semaphore;
		};

		class CThreadPool
		{
		public:
			CThreadPool() = delete;
			CThreadPool(const int32& _threadAmount);
		private:
			std::vector<CThread> m_threads;
			int32 m_threadAmount;
			std::queue<CJob> m_jobQueue;
			std::queue<int32> m_freeThreadIndices;
		};
	}
}