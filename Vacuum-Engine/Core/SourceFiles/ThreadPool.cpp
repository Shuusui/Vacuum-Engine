#include "../HeaderFiles/ThreadPool.h"

namespace Vacuum
{
	namespace Core
	{
		CThreadPool::CThreadPool(const int32& _threadCount)
		{
			bool bIsLowerThanHardwareConcurrency = _threadCount < std::thread::hardware_concurrency();
			for (int32 i = 0; i < bIsLowerThanHardwareConcurrency? _threadCount : std::thread::hardware_concurrency(); ++i)
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