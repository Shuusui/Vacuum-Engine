#pragma once

#include "GlobalDefs.h"
#include <unordered_map>
#include <thread>

namespace Vacuum
{
	namespace Core
	{
		class CThreadPool
		{
		public:
			CThreadPool() = delete;
			CThreadPool(const int32& _threadCount);
			~CThreadPool();
		private:
			std::unordered_map<std::thread::id, std::thread> m_threads;
		};
	}
}