#pragma once

namespace Vacuum
{
	namespace Core
	{
		/**
		 * Don't use this class directly, please use derived classes to create jobs. This is just the base class for the queue.
		 * You can override this class with the execute function to make more implementations possible.
		 */
		class CBaseJob
		{
		public:
			virtual ~CBaseJob() {};
			virtual void Execute() = 0;
		protected:
			CBaseJob(){};
		private:
		};
	}
}