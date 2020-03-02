#include "../HeaderFiles/ThreadPool.h"

namespace Vacuum
{
	namespace Core
	{
		CThreadPool::CThreadPool(const int32& _threadCount)
		{
			for (int32 i = 0; i < _threadCount; ++i)
			{
				std::thread newThread = std::thread();
				//m_threads.insert(std::make_pair(newThread.get_id(), newThread));
			}
		}

		CThreadPool::~CThreadPool()
		{
			for (std::pair<const std::thread::id, std::thread>& thread : m_threads)
			{
				if (thread.second.joinable())
				{
					thread.second.join();
				}
			}
		}
	}
}