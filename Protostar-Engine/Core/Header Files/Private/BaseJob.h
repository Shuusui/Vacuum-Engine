#pragma once

namespace Protostar
{
	/**
		* Don't use this class directly, please use derived classes to create jobs. This is just the base class for the queue.
		* You can override this class with the execute function to make more implementations possible.
		*/
	class CBaseJob
	{
	public:
		/**
			* Protected default constructor just for inheritance reasons
			*/
		CBaseJob() = default;
		/**
			* virtual destructor for inheritance purposes
			*/
		virtual ~CBaseJob() = default;
		/**
			* pure virtual function which will get called of the thread 
			*/
		virtual void Execute() = 0;
	private:
	};
}