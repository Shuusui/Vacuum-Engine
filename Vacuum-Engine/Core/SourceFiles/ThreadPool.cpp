#include "../HeaderFiles/ThreadPool.h"

namespace Vacuum
{
	namespace Core
	{
		CThreadPool::CThreadPool(const int32& _threadCount)
		{
			for (size_t i = 0; i < std::thread::hardware_concurrency()? _threadCount : std::thread::hardware_concurrency(); ++i)
			{
				//m_threads.insert();
			}
		}

		CThreadPool::~CThreadPool()
		{
			for (std::pair<const std::thread::id, std::unique_ptr<std::thread>>& thread : m_threads)
			{
				if (thread.second->joinable())
				{
					thread.second->join();
				}
			}
		}
	}
}