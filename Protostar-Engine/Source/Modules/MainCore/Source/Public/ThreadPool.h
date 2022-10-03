#pragma once

#include "BaseJob.h"
#include "Logger.h"
#include "GlobalDefinitions.h"
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <queue>
#include <unordered_map>
#include <condition_variable>
#include <mutex>

namespace Protostar::Core
{
	/**
	* Derived class of CBaseJob which will implement basic implementation of the std::package_task to bind functions to it.
	* Derive from it further to implement more custom and advanced threads with custom execute functions.
	*/
		class Job : public BaseJob
		{

		public:

			/**
			* Default constructor
			*/
			Job()
			{

			};

			/**
			* Override of execute function which will get called of the CThread when the job gets passed to a CThread
			*/
			virtual void Execute() override
			{
			}

			virtual ~Job() override
			{
			}
		private:
		};

		class Thread
		{
		public:

			/**
			* Deleted cause it's unnecessary and maybe gets accidentally called
			*/
			Thread() = delete;

			/**
			* The only constructor needed to construct the object
			* @param _threadIndex The index of the thread in the owners container
			* @param _owner The owner of this thread. In this case the CThreadPool object
			*/
			Thread(const s32 _threadIndex, class ThreadPool* _owner, std::condition_variable& _semaphore, std::atomic_bool& _stopThread)
				:m_threadIndex(_threadIndex)
				, m_owner(_owner)
				, m_currentJob(nullptr)
				, m_semaphore(_semaphore)
				, m_stopThread(_stopThread)
				, m_thread(std::bind(&Thread::WorkerRun, this))
			{
			}

			/**
			* move constructor
			* @param _other The object which should get moved into this
			*/
			Thread(Thread&& _other) noexcept
				:m_threadIndex(std::move(_other.m_threadIndex))
				, m_owner(std::move(_other.m_owner))
				, m_currentJob(std::move(_other.m_currentJob))
				, m_semaphore(_other.m_semaphore)
				, m_stopThread(_other.m_stopThread)
				, m_thread(std::move(_other.m_thread))
			{
				_other.m_threadIndex = -1;
				_other.m_owner = nullptr;
				_other.m_currentJob = nullptr;
			}

			/**
			* Destructor which joins the thread to the overlying thread if it's joinable.
			*/
			~Thread()
			{
				if (m_thread.joinable())
				{
					m_thread.join();
				}
				if (m_currentJob)
				{
					delete m_currentJob;
					m_currentJob = nullptr;
				}
			}


		private:

			/**
			* The function which will get called inside the std::thread
			*/
			void WorkerRun();

			s32 m_threadIndex;
			class ThreadPool* m_owner;
			BaseJob* m_currentJob;
			std::condition_variable& m_semaphore;
			std::atomic_bool& m_stopThread;
			std::thread m_thread;
		};

		class ThreadPool
		{
		public:
			/**
			* No default constructor, Threadpool can only get initialized with an amount of threads to create
			*/
			ThreadPool() = delete;

			/**
			* The only constructor which will create the amount of screenshots get passed
			* @param The amount of threads to create
			*/
			ThreadPool(const s32 _threadAmount);

			/**
			* Destructor which will join all threads and delete them
			*/
			~ThreadPool();

			/**
			* Queue a new job to the threadpool which will get processed
			*/
			void QueueJob(BaseJob* _jobToQueue);

			/**
			* Function which will get called from the internal thread to dequeue the threadpool
			* @return The first job in queue
			*/
			BaseJob* DequeueJob();

			/**
			* Does the threadpool currently has jobs queued? (threadsafe)
			* @return If the threadpool has jobs queued or not
			*/
			bool HasQueuedJob();

			/**
			* Function to call to reserve a job of the queue which will decrement the internal queue size before dequeing just for the loop of the thread
			* @return If it could get a job reserved
			*/
			bool ReserveJob();

			/**
			* Predicate function for the semaphore to wait
			* @return If the threads should get stopped
			*/
			bool StopThreads();
		private:
			std::vector<Thread*> m_threads;
			s32 m_threadAmount;
			std::queue<BaseJob*> m_jobQueue;
			std::mutex m_queueLock;
			std::condition_variable m_semaphore;
			std::atomic_bool m_stopThreads;
			std::atomic_int32_t m_queueCount;
		};
}