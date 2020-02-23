#pragma once
#include <Windows.h>
#include <string>

namespace Vacuum
{
	namespace Core
	{
		class CLog
		{
		public: 
			/*
			* Needs to get called to initialize the log in first place
			* @param errorMsg If the calll fails this string will get filled
			* @return if it's successful or not
			*/
			static bool Init(std::string& errorMsg);
			/*
			* registeres a new handle with a guid which will get stored to make broadcast over different handles possible
			* param handleGuid the guid of the handle to call it
			* param outputHandle the actual outputhandle where the log system will log to
			* return if the handle guid already exists this will return false otherwise true
			*/
			static bool RegisterHandle(const GUID& handleGuid, const HANDLE& outputHandle);
		private:
			CLog();
			static CLog* s_logHandle;

		};
	}
}